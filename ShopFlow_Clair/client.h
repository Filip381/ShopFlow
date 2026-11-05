#ifndef CLIENT_H
#define CLIENT_H
#include "types.h"
Client *chercherClient(Boutique *b, int id);
const Client *chercherClientConst(const Boutique *b, int id);
int emailExiste(const Boutique *b, const char *email, int idExclu);
int insererClient(Boutique *b, Client client);
int commandesNonAnnulees(const Client *c);
void ajouterClient(Boutique *b);
void modifierClient(Boutique *b);
void supprimerClient(Boutique *b);
void afficherListeClients(const Boutique *b);
void afficherUnClient(const Boutique *b);
#endif
