#ifndef PRODUIT_H
#define PRODUIT_H
#include "types.h"
Produit *chercherProduit(Boutique *b, const char *reference);
const Produit *chercherProduitConst(const Boutique *b, const char *reference);
int produitUtilise(const Boutique *b, const char *reference);
int insererProduit(Boutique *b, Produit p);
void ajouterProduit(Boutique *b);
void modifierProduit(Boutique *b);
void supprimerProduit(Boutique *b);
void afficherCatalogue(const Boutique *b);
void rechercherProduit(const Boutique *b);
#endif

