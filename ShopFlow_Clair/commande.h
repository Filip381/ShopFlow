#ifndef COMMANDE_H
#define COMMANDE_H
#include "types.h"
Commande *chercherCommande(Boutique *b, int numero, Client **client);
int enregistrerCommande(Boutique *b, int idClient, Date date, const char *statut,
                       const char *refs[], const int qtes[], int nbLignes);
int annulerCommandeInterne(Boutique *b, Commande *c, Client *client);
void recalculerCommande(Commande *c);
void creerCommande(Boutique *b);
void afficherUneCommande(Boutique *b);
void ajouterLigneCommande(Boutique *b);
void supprimerLigneCommande(Boutique *b);
void changerStatutCommande(Boutique *b);
void annulerCommande(Boutique *b);
#endif
