#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "type.h"


void ajouterProduit(void) {
    printf("\n===== AJOUT D'UN PRODUIT =====\n");
    printf("  1. Saisir la référence : \n"); // !!! VERIFIER QUE LA REFERENCE EST UNIQUE !!!
    // if (reference == Produit.reference) {
    // printf("  Cette référence existe déjà, entrez en une nouvelle ! ");
    // }
    //scanf();
    printf("  2. Saisir la désignation : \n");
    //scanf();
    printf("  3. Saisir la catégorie : \n");
    //scanf();
    printf("  4. Saisir le prix unitaire hors taxes : \n");
    //scanf();
    printf("  5. Saisir la quantité : \n");
    //scanf();
    printf("\n  Ce produit a bien été ajouté ! \n");
}

void modifierProduit(void) {
    printf("\n===== MODIFIER UN PRODUITS =====\n");
    printf("  1. Saisir le nouveau prix : \n");
    //scanf();
    printf("  2. Saisir le stock à ce jour : \n");
    //scanf();
    printf("  3. Saisir le nouveau seuil d'alerte : \n");
    //scanf();
    printf("\n  Ce produit a bien été modifié :) \n");
}

void afficherCatalogue(void) {
    printf("\n===== AFFICHAGE DU CATALOGUE =====\n");
}


void supprimerProduit(Boutique *b) {
    char reference[12];
    printf("Saisir la reference du produit a supprimer : ");
    scanf("%11s", reference);
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, reference) == 0) {
            if (b->produits[i].actif == 0) {
                printf("Ce produit est deja supprime.\n");
                return;
            }
            b->produits[i].actif = 0;
            printf("Le produit a ete supprime avec succes.\n");
            return;
        }
    }
    printf("Produit introuvable.\n");
}



void rechercherProduit(void) {
    printf("  [STUB] rechercherProduit - a implementer dans produit.c\n");
}

/*
void supprimerProduit(void) {
    printf("\n===== SUPPRIMER UN PRODUITS =====\n");
    printf("  1. Saisir la référence du produit à supprimer : \n");
    //scanf();
}
void rechercherProduit(void)

*/