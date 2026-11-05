/* types.h - Les 7 structures de donnees exigees par le sujet.
 * Une structure regroupe plusieurs variables sous un meme nom.
 * Exemple : Produit contient reference, prix, quantiteStock, etc.
 * Les pointeurs (*produits, *commandes, *lignes) sont des tableaux dynamiques.
 */
#ifndef TYPES_H
#define TYPES_H

/* Structures partagees entre les modules de ShopFlow. */
typedef struct { int jour, mois, annee; } Date;

typedef struct {
    char rue[80];
    char ville[50];
    char codePostal[6];
    char pays[50];
} Adresse;

typedef struct {
    char reference[12];
    char designation[60];
    char categorie[30];
    float prixUnitaireHT;
    int quantiteStock;
    int seuilAlerte;
    int actif;
} Produit;

typedef struct {
    char refProduit[12];
    char designation[60];
    int quantite;
    float prixUnitaireHT;
    float sousTotalHT;
} LigneCommande;

typedef struct {
    int numero;
    int idClient;
    Date dateCommande;
    char statut[20];
    int nbLignes;
    int capaciteLignes;
    LigneCommande *lignes;
    float montantBrutHT;
    float tauxRemise;
    float montantRemise;
    float montantHT;
    float montantTVA;
    float montantTTC;
    float fraisPort;
    Adresse adresseLivraison;
} Commande;

typedef struct {
    int idClient;
    char nom[50];
    char prenom[50];
    char email[80];
    char telephone[15];
    Adresse adresse;
    int pointsFidelite;
    int nbCommandes;
    int capaciteCommandes;
    Commande *commandes;
    int actif;
} Client;

typedef struct {
    char nom[50];
    Produit *produits;
    int nbProduits;
    int capaciteProduits;
    Client *clients;
    int nbClients;
    int capaciteClients;
    int numeroSuivant;
    int idClientSuivant;
} Boutique;

#endif

