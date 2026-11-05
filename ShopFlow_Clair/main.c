/*
 * SHOPFLOW - main.c
 *
 * Debut de lecture conseille : commencez par ce fichier.
 * 1) main() prepare la boutique et affiche le menu.
 * 2) Les fonctions menuXXX() proposent les differentes actions.
 * 3) Chaque action est definie dans son propre fichier .c.
 *
 * Exemple : menuProduits() -> ajouterProduit() dans produit.c.
 */
#include <stdio.h>
#include "utils.h"
#include "produit.h"
#include "client.h"
#include "commande.h"
#include "statistique.h"
#include "initialisation.h"

/* Une saisie valide renvoie un numero, une fin de fichier renvoie -1. */
static int choixMenu(int max) {
    int choix = 0;
    if (!lireEntier("  Votre choix : ", 0, max, &choix)) return -1;
    return choix;
}
/* Gestion des produits : les "case" correspondent aux touches 1,2,3... */
static void menuProduits(Boutique *b) {
    int choix;
    do {
        puts("\n===== GESTION DES PRODUITS =====");
        puts("  1. Ajouter un produit");
        puts("  2. Modifier un produit");
        puts("  3. Supprimer un produit");
        puts("  4. Afficher le catalogue");
        puts("  5. Rechercher un produit");
        puts("  0. Retour au menu principal");
        choix = choixMenu(5);
        switch (choix) {
            case 1: ajouterProduit(b); break;
            case 2: modifierProduit(b); break;
            case 3: supprimerProduit(b); break;
            case 4: afficherCatalogue(b); break;
            case 5: rechercherProduit(b); break;
            default: break;
        }
    } while (choix > 0);
}
/* Gestion des clients : chaque operation est dans client.c. */
static void menuClients(Boutique *b) {
    int choix;
    do {
        puts("\n===== GESTION DES CLIENTS =====");
        puts("  1. Ajouter un client");
        puts("  2. Modifier un client");
        puts("  3. Supprimer un client");
        puts("  4. Afficher la liste des clients");
        puts("  5. Afficher un client et ses commandes");
        puts("  0. Retour au menu principal");
        choix = choixMenu(5);
        switch (choix) {
            case 1: ajouterClient(b); break;
            case 2: modifierClient(b); break;
            case 3: supprimerClient(b); break;
            case 4: afficherListeClients(b); break;
            case 5: afficherUnClient(b); break;
            default: break;
        }
    } while (choix > 0);
}
/* Gestion des commandes : voir commande.c pour les calculs et le stock. */
static void menuCommandes(Boutique *b) {
    int choix;
    do {
        puts("\n===== GESTION DES COMMANDES =====");
        puts("  1. Creer une commande");
        puts("  2. Afficher une commande");
        puts("  3. Ajouter une ligne (EN_COURS)");
        puts("  4. Supprimer une ligne (EN_COURS)");
        puts("  5. Changer le statut");
        puts("  6. Annuler une commande (EN_COURS)");
        puts("  0. Retour au menu principal");
        choix = choixMenu(6);
        switch (choix) {
            case 1: creerCommande(b); break;
            case 2: afficherUneCommande(b); break;
            case 3: ajouterLigneCommande(b); break;
            case 4: supprimerLigneCommande(b); break;
            case 5: changerStatutCommande(b); break;
            case 6: annulerCommande(b); break;
            default: break;
        }
    } while (choix > 0);
}
/* Statistiques : les commandes annulees sont exclues. */
static void menuStatistiques(Boutique *b) {
    int choix;
    do {
        puts("\n===== STATISTIQUES =====");
        puts("  1. Chiffre d'affaires global / par periode");
        puts("  2. Top 5 des produits");
        puts("  3. Classement des clients");
        puts("  4. Statistiques par categorie (section 8)");
        puts("  0. Retour au menu principal");
        choix = choixMenu(4);
        switch (choix) {
            case 1: afficherChiffreAffaires(b); break;
            case 2: palmaresProduits(b); break;
            case 3: palmaresClients(b); break;
            case 4: statistiquesCategories(b); break;
            default: break;
        }
    } while (choix > 0);
}
int main(void) {
    Boutique boutique;
    // Initialisation : 8 produits, 5 clients et 6 commandes.
    if (!initialiserDonneesTest(&boutique)) {
        fputs("[ERREUR] Initialisation impossible.\n", stderr);
        detruireBoutique(&boutique);
        return 1;
    }
    puts("==============================================");
    puts("  SHOPFLOW - Gestion d'une boutique en ligne");
    puts("  Donnees de demonstration chargees.");
    puts("==============================================");
    int choix;
    do {
        puts("\n===== MENU PRINCIPAL =====");
        puts("  1. Gestion des produits");
        puts("  2. Gestion des clients");
        puts("  3. Gestion des commandes");
        puts("  4. Statistiques");
        puts("  0. Quitter");
        choix = choixMenu(4); switch (choix) { case 1: menuProduits(&boutique); break;
            case 2: menuClients(&boutique); break;
            case 3: menuCommandes(&boutique); break;
            case 4: menuStatistiques(&boutique); break;
            default: break;
        }
    } while (choix > 0);
    // Il faut liberer les tableaux dynamiques avant de fermer le programme.
    detruireBoutique(&boutique);
    puts("  Au revoir !");
    return 0;
}



