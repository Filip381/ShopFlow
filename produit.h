#ifndef PRODUIT_H
#define PRODUIT_H

#include "type.h"

//Ajouter un produit
void ajouterProduit(Boutique *b);

//Modifier un produit
void modifierProduit(Boutique *b);

//Supprimer un produit
void supprimerProduit(Boutique *b);

//Afficher le catalogue
void afficherCatalogue(Boutique *b);

//Rechercher un produit
void rechercherProduit(Boutique *b);

#endif
