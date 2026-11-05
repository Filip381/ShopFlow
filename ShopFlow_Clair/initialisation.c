/* initialisation.c - Donnees de demonstration et liberation memoire.
 * Ces donnees sont creees a chaque lancement du logiciel.
 */
#include "initialisation.h"
#include "client.h"
#include "commande.h"
#include "produit.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Jeu de demonstration : 8 produits, 5 clients et 6 commandes (4 statuts).
 * Les quantites du catalogue sont les stocks AVANT les commandes de test. */
/* Premier appel : allouer les produits, les clients et les commandes. */
int initialiserDonneesTest(Boutique *b) {
    memset(b, 0, sizeof *b);
    copierChaine(b->nom, sizeof b->nom, "ShopFlow");
    b->numeroSuivant = 1001;
    b->idClientSuivant = 1;
    Produit modeles[] = {
        {"PRD-0001", "Clavier mecanique",   "Informatique",  79.90f, 42, 10, 1},
        {"PRD-0002", "Souris sans fil",     "Informatique",  24.50f,  8, 15, 1},
        {"PRD-0003", "Ecran 27 pouces",     "Informatique", 199.00f,  6,  5, 1},
        {"PRD-0004", "Casque audio",        "Audio",         89.90f, 25, 10, 1},
        {"PRD-0005", "Enceinte Bluetooth",  "Audio",         59.90f, 12,  8, 1},
        {"PRD-0006", "Cable HDMI 2 m",      "Accessoires",    9.90f,120, 30, 1},
        {"PRD-0007", "Hub USB-C 7 ports",   "Accessoires",   49.90f,  4, 10, 1},
        {"PRD-0008", "Disque SSD 1 To",     "Stockage",     109.00f, 18,  6, 1}
    };
    for (size_t i = 0; i < sizeof modeles / sizeof modeles[0]; i++)
        if (!insererProduit(b, modeles[i])) return 0;
    Client modelesClients[] = {
        {.nom="Bernard", .prenom="Julie", .email="julie.bernard@example.fr", .telephone="0612345678",
         .adresse={"12 rue de la Paix", "Paris", "75002", "France"}},
        {.nom="Martin", .prenom="Lucas", .email="lucas.martin@example.fr", .telephone="0623456789",
         .adresse={"20 avenue Victor Hugo", "Paris", "75016", "France"}},
        {.nom="Dubois", .prenom="Emma", .email="emma.dubois@example.fr", .telephone="0634567890",
         .adresse={"7 rue de Lyon", "Lyon", "69003", "France"}},
        {.nom="Petit", .prenom="Hugo", .email="hugo.petit@example.fr", .telephone="0645678901",
         .adresse={"5 place Bellecour", "Lyon", "69002", "France"}},
        {.nom="Laurent", .prenom="Lea", .email="lea.laurent@example.fr", .telephone="0656789012",
         .adresse={"8 cours Gambetta", "Bordeaux", "33000", "France"}}
    };
    for (size_t i = 0; i < sizeof modelesClients / sizeof modelesClients[0]; i++)
        if (!insererClient(b, modelesClients[i])) return 0;
    /* Commandes #1001 et #1004 ont au moins 3 lignes ; #1001 depasse 600 HT. */
    const char *r1[]={"PRD-0003","PRD-0008","PRD-0001"}; int q1[]={2,2,2};
    const char *r2[]={"PRD-0002","PRD-0006"}; int q2[]={2,4};
    const char *r3[]={"PRD-0004","PRD-0005"}; int q3[]={1,1};
    const char *r4[]={"PRD-0002","PRD-0006","PRD-0007"}; int q4[]={1,3,1};
    const char *r5[]={"PRD-0008","PRD-0004"}; int q5[]={1,2};
    const char *r6[]={"PRD-0001","PRD-0006","PRD-0005"}; int q6[]={1,2,1};
    if (!enregistrerCommande(b,1,(Date){12,3,2026},"LIVREE",r1,q1,3)) return 0;
    if (!enregistrerCommande(b,2,(Date){19,4,2026},"EXPEDIEE",r2,q2,2)) return 0;
    if (!enregistrerCommande(b,3,(Date){2,6,2026},"EN_COURS",r3,q3,2)) return 0;
    if (!enregistrerCommande(b,4,(Date){18,7,2026},"ANNULEE",r4,q4,3)) return 0;
    if (!enregistrerCommande(b,5,(Date){5,9,2026},"LIVREE",r5,q5,2)) return 0;
    if (!enregistrerCommande(b,1,(Date){15,10,2026},"EN_COURS",r6,q6,3)) return 0;
    return 1;
}
/* Dernier appel : free des lignes, puis commandes, puis clients, produits. */
void detruireBoutique(Boutique *b) {
    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++)
            free(b->clients[i].commandes[j].lignes);
        free(b->clients[i].commandes);
    }
    free(b->clients);
    free(b->produits);
    memset(b, 0, sizeof *b);
}

