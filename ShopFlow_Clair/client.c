/* client.c - Gestion des clients.
 * Chaque client possede une adresse et son tableau de commandes.
 * Suppression logique : on conserve les anciennes donnees.
 */
#include "client.h"
#include "commande.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

/* Recherche simple avec une boucle dans le tableau des clients. */
Client *chercherClient(Boutique *b, int id) {
    for (int i = 0; i < b->nbClients; i++)
        if (b->clients[i].idClient == id) return &b->clients[i];
    return NULL;
}
const Client *chercherClientConst(const Boutique *b, int id) {
    for (int i = 0; i < b->nbClients; i++)
        if (b->clients[i].idClient == id) return &b->clients[i];
    return NULL;
}
int emailExiste(const Boutique *b, const char *email, int idExclu) {
    for (int i = 0; i < b->nbClients; i++)
        if (b->clients[i].idClient != idExclu && comparerSansCasse(b->clients[i].email, email) == 0) return 1;
    return 0;
}
/* Attribuer automatiquement un nouvel ID puis ajouter au tableau. */
int insererClient(Boutique *b, Client c) {
    if (!texteNonVide(c.nom) || !texteNonVide(c.prenom) || !emailValide(c.email) ||
        !telephoneValide(c.telephone) || !codePostalValide(c.adresse.codePostal) ||
        !texteNonVide(c.adresse.rue) || !texteNonVide(c.adresse.ville) ||
        !texteNonVide(c.adresse.pays) || emailExiste(b, c.email, -1)) return 0;
    if (!agrandirTableau((void **)&b->clients, &b->capaciteClients, b->nbClients + 1, sizeof *b->clients)) return 0;
    c.idClient = b->idClientSuivant++;
    c.pointsFidelite = 0;
    c.nbCommandes = 0;
    c.capaciteCommandes = 0;
    c.commandes = NULL;
    c.actif = 1;
    b->clients[b->nbClients++] = c;
    return 1;
}
int commandesNonAnnulees(const Client *c) {
    for (int j = 0; j < c->nbCommandes; j++)
        if (strcmp(c->commandes[j].statut, "ANNULEE") != 0) return 1;
    return 0;
}
static int saisirTelephone(char *dest, size_t taille) {
    for (;;) {
        if (!lireTexte("  Telephone (10 chiffres) : ", dest, taille)) return 0;
        if (telephoneValide(dest)) return 1;
        puts("  [!] Telephone invalide.");
    }
}
static int saisirEmail(const Boutique *b, char *dest, size_t taille, int idExclu) {
    for (;;) {
        if (!lireTexte("  Email : ", dest, taille)) return 0;
        if (emailValide(dest) && !emailExiste(b, dest, idExclu)) return 1;
        puts("  [!] Email invalide ou deja attribue (meme a un client inactif).");
    }
}
/* Demander le nom, le prenom, email, telephone et adresse. */
void ajouterClient(Boutique *b) {
    Client c = {0};
    puts("\n--- Ajouter un client ---");
    if (!lireTexte("  Nom : ", c.nom, sizeof c.nom) || !texteNonVide(c.nom) ||
        !lireTexte("  Prenom : ", c.prenom, sizeof c.prenom) || !texteNonVide(c.prenom)) {
        puts("  [!] Nom et prenom obligatoires."); return;
    }
    if (!saisirEmail(b, c.email, sizeof c.email, -1) ||
        !saisirTelephone(c.telephone, sizeof c.telephone) ||
        !saisirAdresse(&c.adresse)) return;
    if (insererClient(b, c)) printf("  Client ajoute, numero %d.\n", b->clients[b->nbClients - 1].idClient);
    else puts("  [!] Echec de l'ajout du client.");
}
/* Changer les coordonnees sans changer les adresses des anciennes commandes. */
void modifierClient(Boutique *b) {
    int id; Client temp;
    if (!lireEntier("  ID du client a modifier : ", 1, 1000000000, &id)) return;
    Client *c = chercherClient(b, id);
    if (!c || !c->actif) { puts("  [!] Client actif introuvable."); return; }
    temp = *c;
    puts("  Saisissez les nouvelles coordonnees :");
    if (!lireTexte("  Nom : ", temp.nom, sizeof temp.nom) || !texteNonVide(temp.nom) ||
        !lireTexte("  Prenom : ", temp.prenom, sizeof temp.prenom) || !texteNonVide(temp.prenom)) {
        puts("  [!] Nom et prenom obligatoires."); return;
    }
    if (!saisirEmail(b, temp.email, sizeof temp.email, id) ||
        !saisirTelephone(temp.telephone, sizeof temp.telephone) ||
        !saisirAdresse(&temp.adresse)) return;
    /* Mise a jour des coordonnees : l'historique garde ses adresses d'origine. */
    copierChaine(c->nom, sizeof c->nom, temp.nom);
    copierChaine(c->prenom, sizeof c->prenom, temp.prenom);
    copierChaine(c->email, sizeof c->email, temp.email);
    copierChaine(c->telephone, sizeof c->telephone, temp.telephone);
    c->adresse = temp.adresse;
    puts("  Client modifie.");
}
/* Conserver la fiche mais passer actif a 0. */
void supprimerClient(Boutique *b) {
    int id;
    if (!lireEntier("  ID du client a desactiver : ", 1, 1000000000, &id)) return;
    Client *c = chercherClient(b, id);
    if (!c || !c->actif) { puts("  [!] Client actif introuvable."); return; }
    if (commandesNonAnnulees(c)) { puts("  [!] Client ayant des commandes non annulees : suppression refusee."); return; }
    c->actif = 0;
    puts("  Client desactive (suppression logique).");
}
static double caClient(const Client *c) {
    double somme = 0;
    for (int j = 0; j < c->nbCommandes; j++)
        if (strcmp(c->commandes[j].statut, "ANNULEE") != 0) somme += c->commandes[j].montantTTC;
    return somme;
}
/* Parcourir les clients et afficher leur total et points. */
void afficherListeClients(const Boutique *b) {
    puts("\n--- Liste des clients ---");
    puts("  ID     Nom                    Email                        Commandes   CA TTC     Points  Statut");
    for (int i = 0; i < b->nbClients; i++) {
        const Client *c = &b->clients[i];
        printf("  %-6d %-22.22s %-28.28s %5d %12.2f %7d  %s\n",
            c->idClient, c->nom, c->email, c->nbCommandes, caClient(c), c->pointsFidelite,
            c->actif ? "ACTIF" : "INACTIF");
    }
    printf("  %d client(s).\n", b->nbClients);
}
/* Afficher la fiche et les numeros des commandes du client. */
void afficherUnClient(const Boutique *b) {
    int id;
    if (!lireEntier("  ID du client : ", 1, 1000000000, &id)) return;
    const Client *c = chercherClientConst(b, id);
    if (!c) { puts("  [!] Client introuvable."); return; }
    printf("\n--- Fiche client %d ---\n", c->idClient);
    printf("  %s %s | %s | %s\n", c->prenom, c->nom, c->email, c->telephone);
    printf("  Adresse : %s, %s %s, %s\n", c->adresse.rue, c->adresse.codePostal, c->adresse.ville, c->adresse.pays);
    printf("  Statut : %s | Points : %d | CA TTC : %.2f EUR\n",
        c->actif ? "ACTIF" : "INACTIF", c->pointsFidelite, caClient(c));
    printf("  Historique (%d commandes) :\n", c->nbCommandes);
    for (int i = 0; i < c->nbCommandes; i++) {
        const Commande *cmd = &c->commandes[i];
        printf("    #%d - %02d/%02d/%04d - %-10s - %8.2f EUR TTC\n",
            cmd->numero, cmd->dateCommande.jour, cmd->dateCommande.mois, cmd->dateCommande.annee,
            cmd->statut, cmd->montantTTC);
    }
}

