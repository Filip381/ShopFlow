#include <stdio.h>
#include <stdlib.h>
#include "produit.h"
#include "produit.c"
#include "initialisation.h"
Boutique boutique;

static int lireChoix(void)
{
    int choix;
    printf("  Votre choix : ");
    if (scanf("%d", &choix) != 1)
    {
        exit(EXIT_FAILURE); /* entree fermee : on quitte proprement */
    }
    return choix;
}

/* --- Module CLIENTS ------------------------------------------------------- */
void ajouterClient(void)       { printf("  [STUB] ajouterClient - a implementer dans client.c\n"); }
void modifierClient(void)      { printf("  [STUB] modifierClient - a implementer dans client.c\n"); }
void supprimerClient(void)     { printf("  [STUB] supprimerClient - a implementer dans client.c\n"); }
void afficherListeClients(void){ printf("  [STUB] afficherListeClients - a implementer dans client.c\n"); }
void afficherClient(void)      { printf("  [STUB] afficherClient - a implementer dans client.c\n"); }

/* --- Module COMMANDES ----------------------------------------------------- */
void creerCommande(void)       { printf("  [STUB] creerCommande - a implementer dans commande.c\n"); }
void ajoutsupCommande(void)    { printf("  [STUB] ajoutsupCommande - a implementer dans commande.c\n"); }
void statusCommande(void)      { printf("  [STUB] statusCommande - a implementer dans commande.c\n"); }
void annuleCommande(void)      { printf("  [STUB] annuleCommande - a implementer dans commande.c\n"); }
void afficherUneCommande(void) { printf("  [STUB] afficherUneCommande - a implementer dans commande.c\n"); }

/* --- Module STATISTIQUES -------------------------------------------------- */
void afficherChiffreAffaires(void) { printf("  [STUB] afficherChiffreAffaires - a implementer dans statistique.c\n"); }
void palmaresProduits(void)        { printf("  [STUB] palmaresProduits - a implementer dans statistique.c\n"); }
void palmaresClients(void)        { printf("  [STUB] palmaresClients - a implementer dans statistique.c\n"); }
void alerteStock(void)            { printf("  [STUB] alerteStock - a implementer dans statistique.c\n"); }

static void menuProduits(Boutique *b)
{
    int choix;
    do
    {
        printf("\n===== GESTION DES PRODUITS =====\n");
        printf("  1. Ajouter un produit\n");
        printf("  2. Modifier un produit\n");
        printf("  3. Supprimer un produit\n");
        printf("  4. Afficher le catalogue\n");
        printf("  5. Rechercher un produit\n");
        printf("  0. Retour au menu principal\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: ajouterProduit(b);    break;
            case 2 : modifierProduit(b);   break;
            case 3: supprimerProduit(b);  break;
            case 4: afficherCatalogue(b); break;
            case 5: rechercherProduit(b); break;
            case 0:                      break; /* retour automatique */
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);
}

static void menuClients(void)
{
    int choix;
    do
    {
        printf("\n===== GESTION DES CLIENTS =====\n");
        printf("  1. Ajouter un client\n");
        printf("  2. Modifier un client\n");
        printf("  3. Supprimer un client\n");
        printf("  4. Afficher la liste des clients\n");
        printf("  5. Afficher un client\n");
        printf("  0. Retour au menu principal\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: ajouterClient();        break;
            case 2: modifierClient();       break;
            case 3: supprimerClient();      break;
            case 4: afficherListeClients(); break;
            case 5: afficherClient();       break;
            case 0:                         break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);
}

static void menuCommandes(void)
{
    int choix;
    do
    {
        printf("\n===== GESTION DES COMMANDES =====\n");
        printf("  1. Creer une commande\n");
        printf("  2. Ajouter ou supprimer une ligne\n");
        printf("  3. Changer le status d'une commande\n");
        printf("  4. Annuler une commande\n");
        printf("  5. Afficher une commande\n");
        printf("  0. Retour au menu principal\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: creerCommande();       break;
            case 2: ajoutsupCommande();     break;
            case 3: statusCommande();    break;
            case 4: annuleCommande();   break;
            case 5: afficherUneCommande(); break;
            case 0:                        break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);
}

static void menuStatistiques(void)
{
    int choix;
    do
    {
        printf("\n===== STATISTIQUES =====\n");
        printf("  1. Chiffre d'affaires global\n");
        printf("  2. Palmarès des produits\n");
        printf("  3. Palmarès des clients\n");
        printf("  4. Alertes de stock\n");
        printf("  0. Retour au menu principal\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: afficherChiffreAffaires(); break;
            case 2: palmaresProduits();        break;
            case 3: palmaresClients();         break;
            case 4: alerteStock();             break;
            case 0:                            break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);
}

/* ==========================================================================
 * PROGRAMME PRINCIPAL
 * ========================================================================== */
int main(void)
{
    int choix;
    Boutique boutique = {0};
    initialiserDonneesTest(&boutique);

    printf("==============================================\n");
    printf("  SHOPFLOW - Boutique en ligne \n");
    printf("==============================================\n");

    do
    {
        printf("\n===== MENU PRINCIPAL =====\n");
        printf("  1. Gestion des produits\n");
        printf("  2. Gestion des clients\n");
        printf("  3. Gestion des commandes\n");
        printf("  4. Statistiques\n");
        printf("  0. Quitter\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: menuProduits(&boutique);     break;
            case 2: menuClients();               break;
            case 3: menuCommandes();             break;
            case 4: menuStatistiques();          break;
            case 0: printf("\n  Au revoir !\n"); break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);

    return EXIT_SUCCESS;
}