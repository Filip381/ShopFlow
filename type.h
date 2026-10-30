/* types.h - structures partagees et constantes de ShopFlow */
#ifndef TYPES_H
#define TYPES_H

/* Statuts possibles d'une commande */
#define STATUT_EN_COURS  "EN_COURS"
#define STATUT_EXPEDIEE  "EXPEDIEE"
#define STATUT_LIVREE    "LIVREE"
#define STATUT_ANNULEE   "ANNULEE"

/* Regles de facturation */
#define TAUX_TVA          20.0f
#define FRAIS_PORT        5.90f
#define SEUIL_PORT_OFFERT 60.0f

/* 3.1.1 Date */
typedef struct {
    int jour;   /* 1..31, valide selon le mois */
    int mois;   /* 1..12 */
    int annee;  /* 2000..2100 */
} Date;

/* 3.1.2 Adresse */
typedef struct {
    char rue[80];
    char ville[50];
    char codePostal[6];  /* 5 chiffres + '\0' */
    char pays[50];
} Adresse;

/* 3.1.3 Produit */
typedef struct {
    char  reference[12];     /* unique, ex. PRD-0001 */
    char  designation[60];
    char  categorie[30];
    float prixUnitaireHT;    /* > 0 */
    int   quantiteStock;     /* >= 0 */
    int   seuilAlerte;       /* >= 0 */
    int   actif;             /* 0 ou 1 (suppression logique) */
} Produit;

/* 3.1.4 LigneCommande */
typedef struct {
    char  refProduit[12];    /* doit exister au catalogue */
    char  designation[60];   /* figee au moment de la vente */
    int   quantite;          /* 1 <= qte <= stock dispo */
    float prixUnitaireHT;    /* fige au moment de la vente */
    float sousTotalHT;       /* quantite * prixUnitaireHT */
} LigneCommande;

/* 3.1.5 Commande */
typedef struct {
    int            numero;            /* unique, > 0 */
    int            idClient;          /* client proprietaire */
    Date           dateCommande;
    char           statut[20];        /* EN_COURS, EXPEDIEE, LIVREE, ANNULEE */
    int            nbLignes;          /* 1 <= nbLignes <= capaciteLignes */
    int            capaciteLignes;
    LigneCommande *lignes;            /* tableau dynamique */
    float          montantBrutHT;     /* somme des sous-totaux */
    float          tauxRemise;        /* 0, 5, 10 ou 15 */
    float          montantRemise;     /* montantBrutHT * tauxRemise / 100 */
    float          montantHT;         /* montantBrutHT - montantRemise */
    float          montantTVA;        /* 20 % du montant HT */
    float          montantTTC;        /* HT + TVA + fraisPort */
    float          fraisPort;         /* 0 ou 5.90 */
    Adresse        adresseLivraison;  /* recopiee a la creation */
} Commande;

/* 3.1.6 Client */
typedef struct {
    int       idClient;           /* unique, > 0 */
    char      nom[50];
    char      prenom[50];
    char      email[80];          /* contient '@', unique */
    char      telephone[15];      /* 10 chiffres */
    Adresse   adresse;
    int       pointsFidelite;     /* >= 0 */
    int       nbCommandes;
    int       capaciteCommandes;
    Commande *commandes;          /* tableau dynamique */
    int       actif;              /* 0 ou 1 (suppression logique) */
} Client;

/* 3.1.7 Boutique (structure racine) */
typedef struct {
    char     nom[50];
    Produit *produits;            /* tableau dynamique */
    int      nbProduits;
    int      capaciteProduits;
    Client  *clients;             /* tableau dynamique */
    int      nbClients;
    int      capaciteClients;
    int      numeroSuivant;       /* prochain numero de commande */
    int      idClientSuivant;     /* prochain identifiant client */
} Boutique;

#endif /* TYPES_H */