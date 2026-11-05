/* produit.c - Catalogue des produits et gestion du stock.
 * Les produits restent en memoire dans le tableau b->produits.
 * Ajouter = agrandir le tableau si necessaire; supprimer = actif = 0.
 */
#include "produit.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

/* Recherche une reference avec une simple boucle for. */
Produit *chercherProduit(Boutique *b, const char *ref) {
    for (int i = 0; i < b->nbProduits; i++)
        if (strcmp(b->produits[i].reference, ref) == 0) return &b->produits[i];
    return NULL;
}
const Produit *chercherProduitConst(const Boutique *b, const char *ref) {
    for (int i = 0; i < b->nbProduits; i++)
        if (strcmp(b->produits[i].reference, ref) == 0) return &b->produits[i];
    return NULL;
}
/* Empeche de desactiver un produit present dans une commande non annulee. */
int produitUtilise(const Boutique *b, const char *ref) {
    for (int i = 0; i < b->nbClients; i++)
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            const Commande *c = &b->clients[i].commandes[j];
            if (strcmp(c->statut, "ANNULEE") == 0) continue;
            for (int k = 0; k < c->nbLignes; k++)
                if (strcmp(c->lignes[k].refProduit, ref) == 0) return 1;
        }
    return 0;
}
/* Ajouter au tableau dynamique : verifier puis agrandir avec realloc. */
int insererProduit(Boutique *b, Produit p) {
    if (!texteNonVide(p.reference) || !texteNonVide(p.designation) || !texteNonVide(p.categorie) ||
        p.prixUnitaireHT <= 0 || p.quantiteStock < 0 || p.seuilAlerte < 0 || chercherProduit(b, p.reference)) return 0;
    if (!agrandirTableau((void **)&b->produits, &b->capaciteProduits,
                        b->nbProduits + 1, sizeof *b->produits)) return 0;
    b->produits[b->nbProduits++] = p;
    return 1;
}
static void afficherProduit(const Produit *p) {
    printf("  %-11s %-24.24s %-15.15s %8.2f %7d %7d %s\n",
        p->reference, p->designation, p->categorie,
        p->prixUnitaireHT, p->quantiteStock, p->seuilAlerte,
        p->actif ? "ACTIF" : "INACTIF");
}
static void entete(void) {
    puts("  Reference   Designation              Categorie       Prix HT    Stock   Alerte Statut");
    puts("  --------------------------------------------------------------------------------------");
}
/* Afficher chaque produit et calculer la valeur du stock actif. */
void afficherCatalogue(const Boutique *b) {
    double valeur = 0.0;
    puts("\n--- Catalogue des produits ---"); entete();
    for (int i = 0; i < b->nbProduits; i++) {
        afficherProduit(&b->produits[i]);
        if (b->produits[i].actif) valeur += b->produits[i].prixUnitaireHT * b->produits[i].quantiteStock;
    }
    printf("  Total : %d produits ; valeur du stock actif : %.2f EUR HT\n", b->nbProduits, valeur);
}
/* Demander les informations a l utilisateur et ajouter le produit. */
void ajouterProduit(Boutique *b) {
    Produit p = {0};
    puts("\n--- Ajouter un produit ---");
    if (!lireTexte("  Reference (11 caracteres max) : ", p.reference, sizeof p.reference)) return;
    if (!texteNonVide(p.reference) || chercherProduit(b, p.reference)) {
        puts("  [!] Reference vide ou deja utilisee (meme si inactive)."); return;
    }
    if (!lireTexte("  Designation : ", p.designation, sizeof p.designation) ||
        !texteNonVide(p.designation)) { puts("  [!] Designation obligatoire."); return; }
    if (!lireTexte("  Categorie : ", p.categorie, sizeof p.categorie) ||
        !texteNonVide(p.categorie)) { puts("  [!] Categorie obligatoire."); return; }
    if (!lireReel("  Prix HT : ", 0, 1000000, &p.prixUnitaireHT) ||
        !lireEntier("  Stock : ", 0, 1000000, &p.quantiteStock) ||
        !lireEntier("  Seuil d'alerte : ", 0, 1000000, &p.seuilAlerte)) return;
    p.actif = 1;
    puts(insererProduit(b, p) ? "  Produit ajoute." : "  [!] Impossible d'ajouter le produit.");
}
/* Remplacer le prix et les quantites du produit choisi. */
void modifierProduit(Boutique *b) {
    char ref[12]; float prix; int stock, seuil;
    if (!lireTexte("  Reference a modifier : ", ref, sizeof ref)) return;
    Produit *p = chercherProduit(b, ref);
    if (!p || !p->actif) { puts("  [!] Produit actif introuvable."); return; }
    printf("  Produit : %s (prix actuel %.2f, stock %d)\n", p->designation, p->prixUnitaireHT, p->quantiteStock);
    if (!lireReel("  Nouveau prix HT : ", 0, 1000000, &prix) ||
        !lireEntier("  Nouveau stock : ", 0, 1000000, &stock) ||
        !lireEntier("  Nouveau seuil : ", 0, 1000000, &seuil)) return;
    p->prixUnitaireHT = prix; p->quantiteStock = stock; p->seuilAlerte = seuil;
    puts("  Produit modifie (les anciennes commandes gardent leurs prix d'origine).");
}
/* Suppression logique : actif = 0, sans free du produit individuel. */
void supprimerProduit(Boutique *b) {
    char ref[12];
    if (!lireTexte("  Reference a desactiver : ", ref, sizeof ref)) return;
    Produit *p = chercherProduit(b, ref);
    if (!p || !p->actif) { puts("  [!] Produit actif introuvable."); return; }
    if (produitUtilise(b, ref)) { puts("  [!] Impossible : produit utilise dans une commande non annulee."); return; }
    p->actif = 0; puts("  Produit desactive (suppression logique).");
}
/* Recherche par reference ou par une partie du nom. */
void rechercherProduit(const Boutique *b) {
    char mot[60]; int n = 0;
    if (!lireTexte("  Reference ou mot-cle : ", mot, sizeof mot)) return;
    if (!texteNonVide(mot)) { puts("  [!] Recherche vide."); return; }
    entete();
    for (int i = 0; i < b->nbProduits; i++) {
        const Produit *p = &b->produits[i];
        if (comparerSansCasse(p->reference, mot) == 0 || contientSansCasse(p->designation, mot)) {
            afficherProduit(p); n++;
        }
    }
    printf("  %d resultat(s).\n", n);
}

