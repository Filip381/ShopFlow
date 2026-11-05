#include "commande.h"
#include "client.h"
#include "produit.h"
#include "utils.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Calcul d'une commande : exemple avec 150 EUR HT brut :
 * remise 5 %, puis TVA 20 %, puis frais de livraison eventuels.
 * La fonction est appelee chaque fois qu'une ligne change.
 */
void recalculerCommande(Commande *c) {
    // ETAPE 1 : additionner les sous-totaux des produits
    c->montantBrutHT = 0;
    for (int i = 0; i < c->nbLignes; i++) {
        c->montantBrutHT += c->lignes[i].sousTotalHT;
    }

    // ETAPE 2 : choisir la reduction selon le montant AVANT reduction
    float brut = c->montantBrutHT;
    if (brut >= 600) {
        c->tauxRemise = 15;
    } else if (brut >= 300) {
        c->tauxRemise = 10;
    } else if (brut >= 100) {
        c->tauxRemise = 5;
    } else {
        c->tauxRemise = 0;
    }

    // ETAPE 3 : calculer les valeurs de la facture
    c->montantRemise = brut * c->tauxRemise / 100.0f;
    c->montantHT = brut - c->montantRemise;
    c->montantTVA = c->montantHT * 0.20f;

    // ETAPE 4 : livraison offerte a partir de 60 EUR brut HT
    if (brut >= 60) {
        c->fraisPort = 0.0f;
    } else {
        c->fraisPort = 5.90f;
    }
    // Pas de TVA sur les frais de livraison dans cet exercice
    c->montantTTC = c->montantHT + c->montantTVA + c->fraisPort;
}

/* Partie entiere de la facture TTC divisee par 10 : 89 EUR = 8 points. */
static double calculPoints(const Commande *c) {
    return ((double)c->montantTTC + 0.0001) / 10.0;
}

static int pointsCommande(const Commande *c) {
    return (int)calculPoints(c);
}
/* Parcourt les clients, puis leurs commandes pour retrouver le numero. */
Commande *chercherCommande(Boutique *b, int numero, Client **client) {
    for (int i = 0; i < b->nbClients; i++)
        for (int j = 0; j < b->clients[i].nbCommandes; j++)
            if (b->clients[i].commandes[j].numero == numero) {
                if (client) *client = &b->clients[i];
                return &b->clients[i].commandes[j];
            }
    if (client) *client = NULL;
    return NULL;
}

/* Reservation du stock pendant la creation ou l'edition d'une commande. */
/* Controle le stock et les doublons, ajoute la ligne avec realloc,
 * puis enleve la quantite du stock. Retourne 1 si tout va bien. */
static int insererLigne(Boutique *b, Commande *c, const char *ref, int qte,
                        int pointsDisponibles) {
    Produit *p = chercherProduit(b, ref);
    if (!p || !p->actif || qte < 1 || qte > p->quantiteStock) return 0;
    for (int i = 0; i < c->nbLignes; i++)
        if (strcmp(c->lignes[i].refProduit, ref) == 0) return 0;
    if (!agrandirTableau((void **)&c->lignes, &c->capaciteLignes,
                        c->nbLignes + 1, sizeof *c->lignes)) return 0;
    LigneCommande *ligne = &c->lignes[c->nbLignes++];
    memset(ligne, 0, sizeof *ligne);
    copierChaine(ligne->refProduit, sizeof ligne->refProduit, p->reference);
    copierChaine(ligne->designation, sizeof ligne->designation, p->designation);
    ligne->quantite = qte;
    ligne->prixUnitaireHT = p->prixUnitaireHT;
    ligne->sousTotalHT = qte * p->prixUnitaireHT;
    recalculerCommande(c);
    /* Verifier les points avant de modifier le stock ou le solde du client. */
    if (calculPoints(c) >= (double)pointsDisponibles + 1.0) {
        c->nbLignes--;
        recalculerCommande(c);
        puts("  [!] Le total de points de fidelite depasserait la limite.");
        return 0;
    }
    p->quantiteStock -= qte;
    return 1;
}
/* Reajoute les quantites au stock (si creation annulee ou commande annulee). */
static void restaurerStock(Boutique *b, const Commande *c) {
    for (int i = 0; i < c->nbLignes; i++) {
        Produit *p = chercherProduit(b, c->lignes[i].refProduit);
        if (p) p->quantiteStock += c->lignes[i].quantite;
    }
}

/* Cree une commande et prend possession du tableau de lignes en memoire. */
int enregistrerCommande(Boutique *b, int idClient, Date date, const char *statut,
                       const char *refs[], const int qtes[], int nbLignes) {
    Client *client = chercherClient(b, idClient);
    if (!client || !client->actif || !dateValide(date) || nbLignes < 1 || !refs || !qtes ||
        (strcmp(statut, "EN_COURS") && strcmp(statut, "EXPEDIEE") &&
         strcmp(statut, "LIVREE") && strcmp(statut, "ANNULEE"))) return 0;
    Commande c = {0};
    c.dateCommande = date;
    c.idClient = idClient;
    c.adresseLivraison = client->adresse;
    copierChaine(c.statut, sizeof c.statut, "EN_COURS");
    for (int i = 0; i < nbLignes; i++) {
        if (!insererLigne(b, &c, refs[i], qtes[i], INT_MAX - client->pointsFidelite)) {
            restaurerStock(b, &c);
            free(c.lignes);
            return 0;
        }
    }
    if (!agrandirTableau((void **)&client->commandes, &client->capaciteCommandes,
                         client->nbCommandes + 1, sizeof *client->commandes)) {
        restaurerStock(b, &c);
        free(c.lignes);
        return 0;
    }
    c.numero = b->numeroSuivant++;
    copierChaine(c.statut, sizeof c.statut, statut);
    if (strcmp(statut, "ANNULEE") == 0) restaurerStock(b, &c);
    else client->pointsFidelite += pointsCommande(&c);
    client->commandes[client->nbCommandes++] = c;
    return c.numero;
}
/* Annulation seulement si EN_COURS : remettre le stock et retirer les points. */
int annulerCommandeInterne(Boutique *b, Commande *c, Client *client) {
    if (strcmp(c->statut, "EN_COURS") != 0) return 0;
    restaurerStock(b, c);
    client->pointsFidelite -= pointsCommande(c);
    if (client->pointsFidelite < 0) client->pointsFidelite = 0;
    copierChaine(c->statut, sizeof c->statut, "ANNULEE");
    return 1;
}
static void afficherCommande(const Commande *c) {
    printf("\n--- Commande #%d ---\n", c->numero);
    printf("  Client %d | Date %02d/%02d/%04d | Statut %s\n", c->idClient,
           c->dateCommande.jour, c->dateCommande.mois, c->dateCommande.annee, c->statut);
    printf("  Livraison : %s, %s %s, %s\n", c->adresseLivraison.rue,
           c->adresseLivraison.codePostal, c->adresseLivraison.ville, c->adresseLivraison.pays);
    puts("  Ref.       Designation                   Qte   Prix HT  Sous-total HT");
    for (int i = 0; i < c->nbLignes; i++) {
        const LigneCommande *l = &c->lignes[i];
        printf("  %-11s %-27.27s %4d %9.2f %14.2f\n",
            l->refProduit, l->designation, l->quantite, l->prixUnitaireHT, l->sousTotalHT);
    }
    printf("  Brut HT :      %.2f EUR\n", c->montantBrutHT);
    printf("  Remise %.0f%% :   -%.2f EUR\n", c->tauxRemise, c->montantRemise);
    printf("  Net HT :       %.2f EUR\n", c->montantHT);
    printf("  TVA 20%% :      %.2f EUR\n", c->montantTVA);
    printf("  Livraison :    %.2f EUR\n", c->fraisPort);
    printf("  TOTAL TTC :    %.2f EUR\n", c->montantTTC);
}
/* Saisie au clavier d'une nouvelle commande de N lignes. */
void creerCommande(Boutique *b) {
    int id, n;
    if (!lireEntier("  ID client : ", 1, 1000000000, &id)) return;
    Client *cl = chercherClient(b, id);
    if (!cl || !cl->actif) { puts("  [!] Client actif introuvable."); return; }
    Date date;
    puts("  Date de la commande :");
    if (!saisirDate(&date) || !lireEntier("  Nombre de lignes (1-100) : ", 1, 100, &n)) return;
    Commande temp = {0};
    for (int i = 0; i < n; i++) {
        char ref[12]; int qte;
        printf("  Ligne %d/%d\n", i + 1, n);
        if (!lireTexte("  Reference produit : ", ref, sizeof ref) ||
            !lireEntier("  Quantite : ", 1, 1000000, &qte)) {
            restaurerStock(b, &temp);
            free(temp.lignes);
            return;
        }
        if (!insererLigne(b, &temp, ref, qte, INT_MAX - cl->pointsFidelite)) {
            puts("  [!] Produit inconnu/inactif, stock insuffisant ou reference deja utilisee. Creation annulee.");
            restaurerStock(b, &temp);
            free(temp.lignes);
            return;
        }
    }
    if (!agrandirTableau((void **)&cl->commandes, &cl->capaciteCommandes,
                         cl->nbCommandes + 1, sizeof *cl->commandes)) {
        puts("  [!] Memoire insuffisante.");
        restaurerStock(b, &temp);
        free(temp.lignes);
        return;
    }
    temp.idClient = id;
    temp.numero = b->numeroSuivant++;
    temp.dateCommande = date;
    temp.adresseLivraison = cl->adresse;
    copierChaine(temp.statut, sizeof temp.statut, "EN_COURS");
    cl->pointsFidelite += pointsCommande(&temp);
    cl->commandes[cl->nbCommandes++] = temp;
    printf("  Commande #%d creee. Points gagnes : %d.\n", temp.numero, pointsCommande(&temp));
    afficherCommande(&cl->commandes[cl->nbCommandes - 1]);
}
void afficherUneCommande(Boutique *b) {
    int numero;
    if (!lireEntier("  Numero de commande : ", 1, 1000000000, &numero)) return;
    Commande *c = chercherCommande(b, numero, NULL);
    if (!c) puts("  [!] Commande introuvable.");
    else afficherCommande(c);
}
void ajouterLigneCommande(Boutique *b) {
    int numero, qte; char ref[12]; Client *cl;
    if (!lireEntier("  Numero de commande : ", 1, 1000000000, &numero)) return;
    Commande *c = chercherCommande(b, numero, &cl);
    if (!c || strcmp(c->statut, "EN_COURS")) { puts("  [!] Commande inexistante ou non modifiable."); return; }
    if (!lireTexte("  Reference a ajouter : ", ref, sizeof ref) ||
        !lireEntier("  Quantite : ", 1, 1000000, &qte)) return;
    int anciensPoints = pointsCommande(c);
    int autresPoints = cl->pointsFidelite - anciensPoints;
    if (!insererLigne(b, c, ref, qte, INT_MAX - autresPoints)) {
        puts("  [!] Ligne non ajoutee.");
        return;
    }
    cl->pointsFidelite += pointsCommande(c) - anciensPoints;
    puts("  Ligne ajoutee."); afficherCommande(c);
}
/* Suppression d'une ligne : deplacer les suivantes a gauche avec une boucle. */
void supprimerLigneCommande(Boutique *b) {
    int numero; char ref[12]; Client *cl;
    if (!lireEntier("  Numero de commande : ", 1, 1000000000, &numero)) return;
    Commande *c = chercherCommande(b, numero, &cl);
    if (!c || strcmp(c->statut, "EN_COURS")) { puts("  [!] Commande inexistante ou non modifiable."); return; }
    if (c->nbLignes <= 1) { puts("  [!] Une commande doit conserver au moins une ligne (sinon annulez-la)."); return; }
    if (!lireTexte("  Reference a retirer : ", ref, sizeof ref)) return;
    for (int i = 0; i < c->nbLignes; i++) {
        if (!strcmp(c->lignes[i].refProduit, ref)) {
            int anciensPoints = pointsCommande(c);
            LigneCommande ligneRetiree = c->lignes[i];
            for (int j = i; j + 1 < c->nbLignes; j++) c->lignes[j] = c->lignes[j + 1];
            c->nbLignes--;
            recalculerCommande(c);
            int autresPoints = cl->pointsFidelite - anciensPoints;
            if (calculPoints(c) >= (double)(INT_MAX - autresPoints) + 1.0) {
                for (int j = c->nbLignes; j > i; j--) c->lignes[j] = c->lignes[j - 1];
                c->lignes[i] = ligneRetiree;
                c->nbLignes++;
                recalculerCommande(c);
                puts("  [!] Le total de points de fidelite depasserait la limite.");
                return;
            }
            Produit *p = chercherProduit(b, ref);
            if (p) p->quantiteStock += ligneRetiree.quantite;
            cl->pointsFidelite += pointsCommande(c) - anciensPoints;
            puts("  Ligne retiree, stock restaure."); afficherCommande(c);
            return;
        }
    }
    puts("  [!] Reference absente de cette commande.");
}
/* Changement de statut dans un seul sens : EN_COURS -> EXPEDIEE -> LIVREE. */
void changerStatutCommande(Boutique *b) {
    int num; Client *cl;
    if (!lireEntier("  Numero de commande : ", 1, 1000000000, &num)) return;
    Commande *c = chercherCommande(b, num, &cl);
    (void)cl;
    if (!c) { puts("  [!] Commande introuvable."); return; }
    if (!strcmp(c->statut, "EN_COURS")) copierChaine(c->statut, sizeof c->statut, "EXPEDIEE");
    else if (!strcmp(c->statut, "EXPEDIEE")) copierChaine(c->statut, sizeof c->statut, "LIVREE");
    else { puts("  [!] Transition interdite (LIVREE ou ANNULEE)."); return; }
    printf("  Nouveau statut : %s\n", c->statut);
}
/* Point d'entree du menu pour annuler une commande en cours. */
void annulerCommande(Boutique *b) {
    int num; Client *cl;
    if (!lireEntier("  Numero de commande : ", 1, 1000000000, &num)) return;
    Commande *c = chercherCommande(b, num, &cl);
    if (!c || !annulerCommandeInterne(b, c, cl)) {
        puts("  [!] Seule une commande EN_COURS peut etre annulee."); return;
    }
    puts("  Commande annulee : stock et points de fidelite restitues.");
}

