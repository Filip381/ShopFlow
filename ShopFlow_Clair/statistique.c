/*
 * statistique.c - Module 4 : statistiques de la boutique.
 * Les commandes ANNULEE ne comptent jamais dans les calculs.
 * Les tris sont faits avec une boucle simple (tri par selection).
 */
#include "statistique.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const Produit *produit;
    int quantite;
} ScoreProduit;

typedef struct {
    const Client *client;
    double ca;
} ScoreClient;

typedef struct {
    char nom[30];
    double caHT;
    int unites;
} ScoreCategorie;

/* Une fonction qui renvoie 1 si la commande compte dans les statistiques. */
static int commandeValide(const Commande *commande) {
    return strcmp(commande->statut, "ANNULEE") != 0;
}

/* Somme des commandes avec filtres facultatifs : 0 = tous. */
void afficherChiffreAffaires(const Boutique *b) {
    double totalHT = 0;
    double totalTTC = 0;
    int mois, annee;
    int nombre = 0;

    if (!lireEntier("  Mois (0 = tous) : ", 0, 12, &mois)) return;
    if (!lireEntier("  Annee (0 = toutes) : ", 0, 2100, &annee)) return;
    if (annee > 0 && annee < 2000) {
        puts("  [!] Choisissez 0 ou une annee entre 2000 et 2100.");
        return;
    }

    for (int i = 0; i < b->nbClients; i++) {
        const Client *client = &b->clients[i];
        for (int j = 0; j < client->nbCommandes; j++) {
            const Commande *commande = &client->commandes[j];
            if (!commandeValide(commande)) continue;
            if (mois != 0 && commande->dateCommande.mois != mois) continue;
            if (annee != 0 && commande->dateCommande.annee != annee) continue;
            totalHT += commande->montantHT;
            totalTTC += commande->montantTTC;
            nombre++;
        }
    }
    printf("  Nombre de commandes : %d\n", nombre);
    printf("  Chiffre d'affaires HT  : %.2f EUR\n", totalHT);
    printf("  Chiffre d'affaires TTC : %.2f EUR\n", totalTTC);
}

/* Classer les produits selon la quantite totale vendue. */
void palmaresProduits(const Boutique *b) {
    int nombre = b->nbProduits;
    ScoreProduit *tableau = calloc((size_t)(nombre > 0 ? nombre : 1), sizeof(ScoreProduit));
    if (tableau == NULL) {
        puts("  [!] Memoire insuffisante.");
        return;
    }

    // 1. Associer un score a chaque produit
    for (int i = 0; i < nombre; i++) {
        tableau[i].produit = &b->produits[i];
        tableau[i].quantite = 0;
    }
    // 2. Lire les lignes de toutes les commandes non annulees
    for (int i = 0; i < b->nbClients; i++) {
        const Client *client = &b->clients[i];
        for (int j = 0; j < client->nbCommandes; j++) {
            const Commande *commande = &client->commandes[j];
            if (!commandeValide(commande)) continue;
            for (int k = 0; k < commande->nbLignes; k++) {
                const LigneCommande *ligne = &commande->lignes[k];
                for (int p = 0; p < nombre; p++) {
                    if (strcmp(tableau[p].produit->reference, ligne->refProduit) == 0) {
                        tableau[p].quantite += ligne->quantite;
                        break;
                    }
                }
            }
        }
    }
    // 3. Tri par selection : chercher le meilleur score et l'echanger
    for (int i = 0; i < nombre; i++) {
        int meilleur = i;
        for (int j = i + 1; j < nombre; j++) {
            if (tableau[j].quantite > tableau[meilleur].quantite ||
                (tableau[j].quantite == tableau[meilleur].quantite &&
                 strcmp(tableau[j].produit->reference, tableau[meilleur].produit->reference) < 0)) {
                meilleur = j;
            }
        }
        ScoreProduit temp = tableau[i];
        tableau[i] = tableau[meilleur];
        tableau[meilleur] = temp;
    }
    puts("\n--- TOP 5 DES PRODUITS ---");
    int limite = nombre < 5 ? nombre : 5;
    for (int i = 0; i < limite; i++) {
        printf("  %d. %-12s %-25.25s %d unites\n", i + 1,
               tableau[i].produit->reference, tableau[i].produit->designation,
               tableau[i].quantite);
    }
    free(tableau);
}

/* Classer les clients en fonction de leur montant total TTC. */
void palmaresClients(const Boutique *b) {
    int nombre = b->nbClients;
    ScoreClient *tableau = calloc((size_t)(nombre > 0 ? nombre : 1), sizeof(ScoreClient));
    if (tableau == NULL) {
        puts("  [!] Memoire insuffisante.");
        return;
    }
    for (int i = 0; i < nombre; i++) {
        tableau[i].client = &b->clients[i];
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            const Commande *commande = &b->clients[i].commandes[j];
            if (commandeValide(commande)) {
                tableau[i].ca += commande->montantTTC;
            }
        }
    }
    // Tri par selection : le client avec le meilleur CA en premier
    for (int i = 0; i < nombre; i++) {
        int meilleur = i;
        for (int j = i + 1; j < nombre; j++) {
            if (tableau[j].ca > tableau[meilleur].ca ||
                (tableau[j].ca == tableau[meilleur].ca &&
                 tableau[j].client->idClient < tableau[meilleur].client->idClient)) {
                meilleur = j;
            }
        }
        ScoreClient temp = tableau[i];
        tableau[i] = tableau[meilleur];
        tableau[meilleur] = temp;
    }
    puts("\n--- CLASSEMENT DES CLIENTS ---");
    for (int i = 0; i < nombre; i++) {
        printf("  %d. Client #%d - %-18.18s %-18.18s CA TTC : %.2f EUR\n",
               i + 1, tableau[i].client->idClient,
               tableau[i].client->nom, tableau[i].client->prenom, tableau[i].ca);
    }
    free(tableau);
}

/* BONUS 2 : pour chaque categorie, calculer les ventes HT apres remise. */
void statistiquesCategories(const Boutique *b) {
    int capacite = b->nbProduits > 0 ? b->nbProduits : 1;
    ScoreCategorie *tableau = calloc((size_t)capacite, sizeof(ScoreCategorie));
    if (tableau == NULL) {
        puts("  [!] Memoire insuffisante.");
        return;
    }
    int nombre = 0;
    // 1. Recenser les noms de categories sans doublon
    for (int i = 0; i < b->nbProduits; i++) {
        int presente = 0;
        for (int j = 0; j < nombre; j++) {
            if (strcmp(tableau[j].nom, b->produits[i].categorie) == 0) {
                presente = 1;
                break;
            }
        }
        if (!presente) {
            copierChaine(tableau[nombre].nom, sizeof(tableau[nombre].nom), b->produits[i].categorie);
            nombre++;
        }
    }
    // 2. Pour chaque ligne de commande, retrouver la categorie du produit
    for (int i = 0; i < b->nbClients; i++) {
        const Client *client = &b->clients[i];
        for (int j = 0; j < client->nbCommandes; j++) {
            const Commande *commande = &client->commandes[j];
            if (!commandeValide(commande)) continue;
            for (int k = 0; k < commande->nbLignes; k++) {
                const LigneCommande *ligne = &commande->lignes[k];
                for (int p = 0; p < b->nbProduits; p++) {
                    const Produit *produit = &b->produits[p];
                    if (strcmp(produit->reference, ligne->refProduit) != 0) continue;
                    for (int z = 0; z < nombre; z++) {
                        if (strcmp(tableau[z].nom, produit->categorie) == 0) {
                            tableau[z].unites += ligne->quantite;
                            tableau[z].caHT += ligne->sousTotalHT * (1.0f - commande->tauxRemise / 100.0f);
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    puts("\n--- VENTES PAR CATEGORIE (BONUS 2) ---");
    for (int i = 0; i < nombre; i++) {
        printf("  %-20.20s %4d unites, %.2f EUR HT\n",
               tableau[i].nom, tableau[i].unites, tableau[i].caHT);
    }
    free(tableau);
}

