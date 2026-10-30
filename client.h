#ifndef CLIENT_H
#define CLIENT_H

#include "types.h"

// Ajouter un client
void ajouterClient(Boutique *b);

// Modifier un client
void modifierClient(Boutique *b);

// Supprimer un client
void supprimerClient(Boutique *b);

// Afficher la liste des clients
void afficherListeClients(const Boutique *b);

// Afficher un client
void afficherUnClient(const Boutique *b);

#endif
