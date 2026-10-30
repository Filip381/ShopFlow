#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "utils.h"


// 1. Ajouter un client
void ajouterClient(Boutique *b) {

    Client c = {0};
    int valide;

    printf("\n--- Ajouter un client ---\n");

    // Nom
    do {
        if (!lireTexte("Nom : ", c.nom, sizeof(c.nom)))
            return;
    } while (!texteNonVide(c.nom));

    // Prenom
    do {
        if (!lireTexte("Prenom : ", c.prenom, sizeof(c.prenom)))
            return;
    } while (!texteNonVide(c.prenom));

    // Email
    do {
        valide = 1;

        if (!lireTexte("Email : ", c.email, sizeof(c.email)))
            return;

        if (!emailValide(c.email))
            valide = 0;

        for (int i = 0; i < b->nbClients; i++) {
            if (comparerSansCasse(b->clients[i].email, c.email) == 0)
                valide = 0;
        }

        if (!valide)
            printf("Email invalide ou deja utilise.\n");

    } while (!valide);

    // Telephone
    do {
        if (!lireTexte("Telephone : ", c.telephone, sizeof(c.telephone)))
            return;

        if (!telephoneValide(c.telephone))
            printf("Telephone invalide.\n");

    } while (!telephoneValide(c.telephone));

    // Adresse
    if (!saisirAdresse(&c.adresse))
        return;

    // Agrandir le tableau si necessaire
    if (b->nbClients >= b->capaciteClients) {

        int nouvelleCapacite = b->capaciteClients + 5;

        Client *temp = realloc(
            b->clients,
            nouvelleCapacite * sizeof(Client)
        );

        if (temp == NULL) {
            printf("Erreur de memoire.\n");
            return;
        }

        b->clients = temp;
        b->capaciteClients = nouvelleCapacite;
    }

    // Initialiser le client
    c.idClient = b->idClientSuivant;
    b->idClientSuivant++;

    c.pointsFidelite = 0;
    c.nbCommandes = 0;
    c.capaciteCommandes = 0;
    c.commandes = NULL;
    c.actif = 1;

    b->clients[b->nbClients] = c;
    b->nbClients++;

    printf("Client ajoute avec succes.\n");
}


// 2. Modifier un client
void modifierClient(Boutique *b) {

    int id;
    int position = -1;
    int valide;

    if (!lireEntier("ID du client : ", 1, 1000000000, &id))
        return;

    for (int i = 0; i < b->nbClients; i++) {
        if (b->clients[i].idClient == id &&
            b->clients[i].actif == 1) {
            position = i;
            break;
        }
    }

    if (position == -1) {
        printf("Client introuvable.\n");
        return;
    }

    Client c = b->clients[position];

    // Modifier le nom
    do {
        if (!lireTexte("Nouveau nom : ", c.nom, sizeof(c.nom)))
            return;
    } while (!texteNonVide(c.nom));

    // Modifier le prenom
    do {
        if (!lireTexte("Nouveau prenom : ", c.prenom, sizeof(c.prenom)))
            return;
    } while (!texteNonVide(c.prenom));

    // Modifier l'email
    do {
        valide = 1;

        if (!lireTexte("Nouvel email : ", c.email, sizeof(c.email)))
            return;

        if (!emailValide(c.email))
            valide = 0;

        for (int i = 0; i < b->nbClients; i++) {
            if (i != position &&
                comparerSansCasse(b->clients[i].email, c.email) == 0)
                valide = 0;
        }

        if (!valide)
            printf("Email invalide ou deja utilise.\n");

    } while (!valide);

    // Modifier le telephone
    do {
        if (!lireTexte("Nouveau telephone : ", c.telephone,
                       sizeof(c.telephone)))
            return;

        if (!telephoneValide(c.telephone))
            printf("Telephone invalide.\n");

    } while (!telephoneValide(c.telephone));

    // Modifier l'adresse
    if (!saisirAdresse(&c.adresse))
        return;

    b->clients[position] = c;

    printf("Client modifie avec succes.\n");
}


// 3. Supprimer un client
void supprimerClient(Boutique *b) {

    int id;

    if (!lireEntier("ID du client : ", 1, 1000000000, &id))
        return;

    for (int i = 0; i < b->nbClients; i++) {

        if (b->clients[i].idClient == id &&
            b->clients[i].actif == 1) {

            // Verifier les commandes
            for (int j = 0; j < b->clients[i].nbCommandes; j++) {

                if (strcmp(b->clients[i].commandes[j].statut,
                           "ANNULEE") != 0) {

                    printf("Suppression impossible : commandes existantes.\n");
                    return;
                }
            }

            // Suppression logique
            b->clients[i].actif = 0;

            printf("Client supprime avec succes.\n");
            return;
        }
    }

    printf("Client introuvable.\n");
}


// 4. Afficher la liste des clients
void afficherListeClients(const Boutique *b) {

    printf("\n--- Liste des clients ---\n");

    if (b->nbClients == 0) {
        printf("Aucun client.\n");
        return;
    }

    for (int i = 0; i < b->nbClients; i++) {

        const Client *c = &b->clients[i];
        float ca = 0;

        // Calcul du chiffre d'affaires
        for (int j = 0; j < c->nbCommandes; j++) {

            if (strcmp(c->commandes[j].statut, "ANNULEE") != 0) {
                ca += c->commandes[j].montantTTC;
            }
        }

        printf("\nID : %d\n", c->idClient);
        printf("Nom : %s\n", c->nom);
        printf("Prenom : %s\n", c->prenom);
        printf("Email : %s\n", c->email);
        printf("Commandes : %d\n", c->nbCommandes);
        printf("Chiffre d'affaires TTC : %.2f EUR\n", ca);
        printf("Statut : %s\n", c->actif ? "ACTIF" : "INACTIF");
    }
}


// 5. Afficher un client
void afficherUnClient(const Boutique *b) {

    int id;

    if (!lireEntier("ID du client : ", 1, 1000000000, &id))
        return;

    for (int i = 0; i < b->nbClients; i++) {

        const Client *c = &b->clients[i];

        if (c->idClient == id) {

            printf("\n--- Informations du client ---\n");

            printf("ID : %d\n", c->idClient);
            printf("Nom : %s\n", c->nom);
            printf("Prenom : %s\n", c->prenom);
            printf("Email : %s\n", c->email);
            printf("Telephone : %s\n", c->telephone);

            printf("Adresse : %s, %s %s, %s\n",
                   c->adresse.rue,
                   c->adresse.codePostal,
                   c->adresse.ville,
                   c->adresse.pays);

            printf("Points de fidelite : %d\n", c->pointsFidelite);
            printf("Statut : %s\n", c->actif ? "ACTIF" : "INACTIF");
            printf("Nombre de commandes : %d\n", c->nbCommandes);

            float ca = 0;

            // Historique des commandes
            for (int j = 0; j < c->nbCommandes; j++) {

                const Commande *cmd = &c->commandes[j];

                printf("\nCommande numero : %d\n", cmd->numero);

                printf("Date : %02d/%02d/%d\n",
                       cmd->dateCommande.jour,
                       cmd->dateCommande.mois,
                       cmd->dateCommande.annee);

                printf("Statut : %s\n", cmd->statut);

                for (int k = 0; k < cmd->nbLignes; k++) {
                    printf("Produit : %s | Quantite : %d | Sous-total HT : %.2f EUR\n",
                           cmd->lignes[k].designation,
                           cmd->lignes[k].quantite,
                           cmd->lignes[k].sousTotalHT);
                }

                printf("Total TTC : %.2f EUR\n", cmd->montantTTC);

                if (strcmp(cmd->statut, "ANNULEE") != 0) {
                    ca += cmd->montantTTC;
                }
            }

            printf("\nChiffre d'affaires TTC : %.2f EUR\n", ca);
            return;
        }
    }

    printf("Client introuvable.\n");
}
