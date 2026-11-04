#include <stdio.h>
#include <stdlib.h>
#include "initialisation.h"

void initialiserDonneesTest(Boutique *b) {
    //Allocation dynamique de 10 emplacements
    b->capaciteProduits = 10;

    b->produits = malloc(b->capaciteProduits * sizeof(Produit));

    if (b->produits == NULL)
    {
        printf("Erreur : allocation memoire impossible.\n");
        b->capaciteProduits = 0;
        return;
    }

    b->nbProduits = 0;

    //Creation des produits de demonstration
    Produit modele[] = {
        {"PRD-0001", "Clavier mecanique", "Informatique", 79.90f, 42, 10, 1},
        {"PRD-0002", "Souris sans fil", "Informatique", 24.50f, 8, 15, 1},
        {"PRD-0003", "Ecran 27 pouces", "Informatique", 199.00f, 6, 5, 1},
        {"PRD-0004", "Casque audio", "Audio", 89.90f, 25, 10, 1},
        {"PRD-0005", "Enceinte Bluetooth", "Audio", 59.90f, 12, 8, 1},
        {"PRD-0006", "Cable HDMI 2 m", "Accessoires", 9.90f, 120, 30, 1},
        {"PRD-0007", "Hub USB-C 7 ports", "Accessoires", 49.90f, 4, 10, 1},
        {"PRD-0008", "Disque SSD 1 To", "Stockage", 109.00f, 18, 6, 1}
    };

    //Copier les produits dans le tableau de la boutique
    for (int i = 0; i < 8; i++) {
        b->produits[b->nbProduits] = modele[i];
        b->nbProduits++;
    }
}
