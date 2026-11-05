/* utils.h - Saisie, validation et fonctions utilitaires partagees. */
#ifndef UTILS_H
#define UTILS_H
#include <stddef.h>
#include "types.h"


int lireTexte(const char *invite, char *destination, size_t taille);
int lireEntier(const char *invite, int min, int max, int *resultat);
int lireReel(const char *invite, float minExclu, float maxInclus, float *resultat);
int saisirAdresse(Adresse *adresse);
int saisirDate(Date *date);


int texteNonVide(const char *texte);
int codePostalValide(const char *texte);
int telephoneValide(const char *texte);
int emailValide(const char *texte);


void copierChaine(char *destination, size_t capacite, const char *source);
int comparerSansCasse(const char *a, const char *b);
int contientSansCasse(const char *texte, const char *mot);


int agrandirTableau(void **tableau, int *capacite, int minimum, size_t tailleElement);

int dateValide(Date date);


#endif

