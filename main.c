/* ==========================================================================
 * menu.c - Squelette de demarrage pour tester le projet (XTI101)
 * --------------------------------------------------------------------------
 * BUT : un programme qui COMPIL ET TOURNE en 1 commande, des le premier jour.
 *
 *
 * CONTENU : navigation complete (4 sous-menus) + bouchons [STUB] pour
 * chaque fonctionnalite. Vous devez changer les stubs,
 * un par un, par leurs vraies fonctions ecrites dans leurs modules :
 *
 * Le cycle de travail (à répéter pour chaque fonctionnalité)
 * Copy1. CHOISIR une fonctionnalité du menu           (ex. « ajouter un produit »)
 * 2. ÉCRIRE la fonction dans son module           (produit.c)
 * 3. DÉCLARER le prototype dans son header        (produit.h)
 * 4. INCLURE le header dans menu_rapide.c         (#include "produit.h")
 * 5. SUPPRIMER le stub correspondant              (la ligne [STUB] ajouterProduit)
 * 6. RECOMPILER et tester
 *
 * ========================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include "produit.h"
#include "produit.h"
Boutique boutique;

/* --------------------------------------------------------------------------
 * Lecture d'un choix au clavier (version simple, suffisant pour demarrer).
 * A remplacer plus tard par une saisie securisee (module utils.c).
 * -------------------------------------------------------------------------- */
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

/* ==========================================================================
 * STUBS : affichent un message pour PROUVER que l'appel arrive.
 * A REMPLACER par les vraies fonctions des modules au fil des seances.
 * ========================================================================== */

/* --- Module PRODUITS ------------------------------------------------------ */
/*
void ajouterProduit(void);
void modifierProduit(void);
void supprimerProduit(void)    { printf("  [STUB] supprimerProduit - a implementer dans produit.c\n"); }
void afficherCatalogue(void);
void rechercherProduit(void)    { printf("  [STUB] rechercheProduit - a implementer dans produit.c\n"); }
*/

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

/* ==========================================================================
 * SOUS-MENUS : un seul motif a retenir, applique 4 fois.
 * Pour ajouter une fonctionnalite : un printf + un case + la borne max.
 * ========================================================================== */

static void menuProduits(void)
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
            case 1: ajouterProduit();    break;
            case 2: modifierProduit();   break;
            case 3: supprimerProduit(&boutique);  break;
            case 4: afficherCatalogue(); break;
            case 5: rechercherProduit(); break;
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

    printf("==============================================\n");
    printf("  SHOPFLOW - Boutique en ligne (squelette)\n");
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
            case 1: menuProduits();     break;
            case 2: menuClients();      break;
            case 3: menuCommandes();    break;
            case 4: menuStatistiques(); break;
            case 0: printf("\n  Au revoir !\n"); break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);

    return EXIT_SUCCESS;
}