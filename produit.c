#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "produit.h"

void ajouterProduit(Boutique *b) {

    Produit nouveauProduit;

    printf("\n===== AJOUT D'UN NOUVEAU PRODUIT =====\n");

    //Saisir la référence
    do {
        printf("1. Saisir la référence : ");
        scanf("%11s", nouveauProduit.reference);

        int i;

        //Verifier si la référence existe déjà
        for (i = 0; i < b->nbProduits; i++) {

            if (strcmp(b->produits[i].reference, nouveauProduit.reference) == 0
                && b->produits[i].actif == 1) {

                printf("Cette référence existe déjà ! Entrez-en une autre.\n");
                break;
            }
        }

        //Fin de la boucle, aucun produit n'a cette référence
        if (i == b->nbProduits) {
            break;
        }

    } while (1);

    //Saisir la désignation
    printf("2. Saisir la désignation : ");
    scanf(" %59[^\n]", nouveauProduit.designation);

    //Saisir la catégorie
    printf("3. Saisir la catégorie : ");
    scanf(" %29[^\n]", nouveauProduit.categorie);

    //Saisir le prix unitaire HT
    do {
        printf("4. Saisir le prix unitaire HT : ");
        scanf("%f", &nouveauProduit.prixUnitaireHT);

        if (nouveauProduit.prixUnitaireHT <= 0) {
            printf("Le prix doit être strictement supérieur à 0.\n");
        }

    } while (nouveauProduit.prixUnitaireHT <= 0);

    //Saisir la quantité en stock
    do {
        printf("5. Saisir la quantité en stock : ");
        scanf("%d", &nouveauProduit.quantiteStock);

        if (nouveauProduit.quantiteStock < 0) {
            printf("La quantité ne peut pas être négative.\n");
        }

    } while (nouveauProduit.quantiteStock < 0);

    //Saisir le seuil d'alerte
    do {
        printf("6. Saisir le seuil d'alerte : ");
        scanf("%d", &nouveauProduit.seuilAlerte);

        if (nouveauProduit.seuilAlerte < 0) {
            printf("Le seuil d'alerte ne peut pas être négatif.\n");
        }

    } while (nouveauProduit.seuilAlerte < 0);

    //Activer le produit
    nouveauProduit.actif = 1;

    //Vérifier si le tableau doit être agrandi
    if (b->nbProduits == b->capaciteProduits) {

        int nouvelleCapacite;

        if (b->capaciteProduits == 0) {
            nouvelleCapacite = 5;
        } else {
            nouvelleCapacite = b->capaciteProduits * 2;
        }

        Produit *temp = realloc(
            b->produits,
            nouvelleCapacite * sizeof(Produit)
        );

        if (temp == NULL) {
            printf("Erreur : impossible d'ajouter le produit.\n");
            return;
        }

        b->produits = temp;
        b->capaciteProduits = nouvelleCapacite;
    }

    //Ajouter le produit dans le tableau
    b->produits[b->nbProduits] = nouveauProduit;
    b->nbProduits++;

    printf("\nCe produit a bien ete ajoute !\n");
}


void modifierProduit(Boutique *b)
{
    char reference[12];
    int i;
    int choix;

    printf("\n===== MODIFICATION D'UN PRODUIT =====\n");

    //Demander la référence du produit
    printf("Saisir la reference du produit : ");
    scanf("%11s", reference);

    //Rechercher le produit
    for (i = 0; i < b->nbProduits; i++)
    {
        if (strcmp(b->produits[i].reference, reference) == 0)
        {
            break;
        }
    }

    //Vérifier si le produit existe et s'il est actif
    if (i == b->nbProduits || b->produits[i].actif == 0)
    {
        printf("Produit introuvable ou desactive.\n");
        return;
    }

    //Afficher les informations actuelles
    printf("\nProduit trouve : %s\n", b->produits[i].designation);
    printf("Prix actuel : %.2f euros\n", b->produits[i].prixUnitaireHT);
    printf("Stock actuel : %d\n", b->produits[i].quantiteStock);
    printf("Seuil d'alerte actuel : %d\n", b->produits[i].seuilAlerte);

    //Menu de modification
    printf("\nQue souhaitez-vous modifier ?\n");
    printf("1. Prix unitaire HT\n");
    printf("2. Quantite en stock\n");
    printf("3. Seuil d'alerte\n");
    printf("0. Annuler\n");

    printf("Votre choix : ");
    scanf("%d", &choix);

    switch (choix)
    {
        case 1:
            do
            {
                printf("Nouveau prix HT : ");
                scanf("%f", &b->produits[i].prixUnitaireHT);

                if (b->produits[i].prixUnitaireHT <= 0)
                {
                    printf("Le prix doit être supérieur à 0.\n");
                }

            } while (b->produits[i].prixUnitaireHT <= 0);

            printf("Prix modifié avec succès !\n");
            break;

        case 2:
            do
            {
                printf("Nouvelle quantité en stock : ");
                scanf("%d", &b->produits[i].quantiteStock);

                if (b->produits[i].quantiteStock < 0)
                {
                    printf("La quantité ne peut pas être négative.\n");
                }

            } while (b->produits[i].quantiteStock < 0);

            printf("Stock modifié avec succès !\n");
            break;

        case 3:
            do
            {
                printf("Nouveau seuil d'alerte : ");
                scanf("%d", &b->produits[i].seuilAlerte);

                if (b->produits[i].seuilAlerte < 0)
                {
                    printf("Le seuil ne peut pas être négatif.\n");
                }

            } while (b->produits[i].seuilAlerte < 0);

            printf("Seuil d'alerte modifié avec succès !\n");
            break;

        case 0:
            printf("Modification annulée\n");
            break;

        default:
            printf("Choix invalide\n");
            break;
    }
}

void supprimerProduit(Boutique *b)
{
    char reference[12];
    int i, j;

    printf("\n===== SUPPRIMER UN PRODUIT =====\n");
    printf("Entrez la référence du produit à supprimer : ");
    scanf("%11s", reference);

    // Recherche du produit
    for (i = 0; i < b->nbProduits; i++)
    {
        if (strcmp(b->produits[i].reference, reference) == 0)
        {
            break;
        }
    }

    //Vérification : produit inexistant
    if (i == b->nbProduits)
    {
        printf("Produit introuvable.\n");
        return;
    }

    //Suppression logique
    b->produits[i].actif = 0;

    printf("Le produit %s a été supprimé avec succès.\n", reference);
}

void afficherCatalogue(Boutique *b) {
    int i, j;
    int *indices;
    int temp;

    printf("\n========== CATALOGUE DES PRODUITS ==========\n");

    //Vérifier s'il existe des produits
    if (b->nbProduits == 0)
    {
        printf("Aucun produit dans le catalogue.\n");
        return;
    }

    //Allocation du tableau temporaire d'indices
    indices = malloc(b->nbProduits * sizeof(int));

    if (indices == NULL)
    {
        printf("Erreur d'allocation memoire.\n");
        return;
    }

    //Remplir le tableau avec les indices des produits actifs
    int nbActifs = 0;

    for (i = 0; i < b->nbProduits; i++)
    {
        if (b->produits[i].actif == 1)
        {
            indices[nbActifs] = i;
            nbActifs++;
        }
    }

    //Trier les indices selon le stock
    for (i = 0; i < nbActifs - 1; i++)
    {
        for (j = 0; j < nbActifs - 1 - i; j++)
        {
            if (b->produits[indices[j]].quantiteStock <
                b->produits[indices[j + 1]].quantiteStock)
            {
                //Permutation
                temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }

    //Afficher les produits classés
    if (nbActifs == 0)
    {
        printf("Aucun produit actif dans le catalogue.\n");
    }
    else
    {
        printf("%12s %25s %20s %12s %10s\n",
               "REFERENCE", "DESIGNATION", "CATEGORIE", "PRIX HT", "STOCK");

        printf("------------------------------------------------------------------------------------\n");

        for (i = 0; i < nbActifs; i++)
        {

            printf("%12s %25s %20s %8.2f EUR     %5d\n",
                   b->produits[indices[i]].reference,
                   b->produits[indices[i]].designation,
                   b->produits[indices[i]].categorie,
                   b->produits[indices[i]].prixUnitaireHT,
                   b->produits[indices[i]].quantiteStock);
        }
    }
    free(indices);
}

void rechercherProduit(Boutique *b) {
    char reference[12];
    int i;

    printf("\n========== RECHERCHER UN PRODUIT ==========\n");

    printf("Entrez la reference exacte du produit : ");
    scanf("%11s", reference);

    //Recherche du produit dans le tableau
    for (i = 0; i < b->nbProduits; i++)
    {
        if (strcmp(b->produits[i].reference, reference) == 0)
        {
            break;
        }
    }

    //Vérifier si le produit existe
    if (i == b->nbProduits)
    {
        printf("\nAucun produit ne correspond à cette référence.\n");
        return;
    }

    //Vérifier si le produit est actif
    if (b->produits[i].actif == 0)
    {
        printf("\nCe produit existe mais il a été supprimé.\n");
        return;
    }

    //Afficher toutes les informations du produit
    printf("\n========== INFORMATIONS DU PRODUIT ==========\n");

    printf("Reference          : %s\n", b->produits[i].reference);
    printf("Designation        : %s\n", b->produits[i].designation);
    printf("Categorie          : %s\n", b->produits[i].categorie);
    printf("Prix unitaire HT   : %.2f EUR\n", b->produits[i].prixUnitaireHT);
    printf("Quantite en stock  : %d\n", b->produits[i].quantiteStock);
    printf("Seuil d'alerte     : %d\n", b->produits[i].seuilAlerte);
    printf("Statut             : Actif\n");

    //Indiquer si le stock est faible
    if (b->produits[i].quantiteStock <= b->produits[i].seuilAlerte)
    {
        printf("\nAttention : stock faible !\n");
    }
}
