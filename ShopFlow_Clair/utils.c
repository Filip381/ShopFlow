/* utils.c - Saisie securisee, validation, dates, textes et tableaux. */
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>

int lireTexte(const char *invite, char *dest, size_t taille) {
    char tampon[512];
    if (taille == 0) return 0;
    for (;;) {
        printf("%s", invite);
        fflush(stdout);
        if (!fgets(tampon, sizeof tampon, stdin)) return 0;
        size_t n = strlen(tampon);
        if (n && tampon[n - 1] == '\n') tampon[--n] = '\0';
        else if (!feof(stdin)) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("  [!] Texte trop long.\n");
            continue;
        }
        if (n && tampon[n - 1] == '\r') tampon[--n] = '\0';
        if (n >= taille) { printf("  [!] Maximum %zu caracteres.\n", taille - 1); continue; }
        memcpy(dest, tampon, n + 1);
        return 1;
    }
}

int lireEntier(const char *invite, int min, int max, int *resultat) {
    char buf[80], *fin;
    long valeur;
    for (;;) {
        if (!lireTexte(invite, buf, sizeof buf)) return 0;
        errno = 0;
        valeur = strtol(buf, &fin, 10);
        while (isspace((unsigned char)*fin)) fin++;
        if (fin == buf || *fin || errno == ERANGE || valeur < min || valeur > max) {
            printf("  [!] Entrez un entier entre %d et %d.\n", min, max);
            continue;
        }
        *resultat = (int)valeur;
        return 1;
    }
}

int lireReel(const char *invite, float minExclu, float maxInclus, float *resultat) {
    char buf[80], *fin;
    float valeur;
    for (;;) {
        if (!lireTexte(invite, buf, sizeof buf)) return 0;
        for (size_t i = 0; buf[i]; i++) if (buf[i] == ',') buf[i] = '.';
        errno = 0;
        valeur = strtof(buf, &fin);
        while (isspace((unsigned char)*fin)) fin++;
        if (fin == buf || *fin || errno == ERANGE || !isfinite(valeur) || valeur <= minExclu || valeur > maxInclus) {
            printf("  [!] Valeur invalide (%.2f < valeur <= %.2f).\n", minExclu, maxInclus);
            continue;
        }
        *resultat = valeur;
        return 1;
    }
}

static int texteObligatoire(const char *invite, char *dest, size_t taille) {
    for (;;) {
        if (!lireTexte(invite, dest, taille)) return 0;
        if (texteNonVide(dest)) return 1;
        puts("  [!] Ce champ ne peut pas etre vide.");
    }
}

int saisirAdresse(Adresse *a) {
    if (!texteObligatoire("  Rue : ", a->rue, sizeof a->rue)) return 0;
    if (!texteObligatoire("  Ville : ", a->ville, sizeof a->ville)) return 0;
    do {
        if (!lireTexte("  Code postal (5 chiffres) : ", a->codePostal, sizeof a->codePostal)) return 0;
        if (codePostalValide(a->codePostal)) break;
        puts("  [!] Le code postal doit contenir 5 chiffres.");
    } while (1);
    return texteObligatoire("  Pays : ", a->pays, sizeof a->pays);
}

int saisirDate(Date *d) {
    do {
        if (!lireEntier("  Jour : ", 1, 31, &d->jour)) return 0;
        if (!lireEntier("  Mois : ", 1, 12, &d->mois)) return 0;
        if (!lireEntier("  Annee : ", 2000, 2100, &d->annee)) return 0;
        if (dateValide(*d)) return 1;
        puts("  [!] Cette date n'existe pas.");
    } while (1);
}

#include <ctype.h>
#include <string.h>

int texteNonVide(const char *texte) {
    while (*texte) { if (!isspace((unsigned char)*texte++)) return 1; }
    return 0;
}

int codePostalValide(const char *str) {
    if (strlen(str) != 5) return 0;
    for (int i = 0; i < 5; i++) if (!isdigit((unsigned char)str[i])) return 0;
    return 1;
}

int telephoneValide(const char *str) {
    if (strlen(str) != 10) return 0;
    for (int i = 0; i < 10; i++) if (!isdigit((unsigned char)str[i])) return 0;
    return 1;
}

int emailValide(const char *str) {
    const char *arobase = strchr(str, '@');
    if (!arobase || arobase == str || !arobase[1] || strchr(arobase + 1, '@')) return 0;
    const char *point = strrchr(arobase, '.');
    if (!point || point == arobase + 1 || !point[1]) return 0;
    for (const char *p = str; *p; p++) if (isspace((unsigned char)*p)) return 0;
    return 1;
}

#include <stdio.h>
#include <ctype.h>

void copierChaine(char *dest, size_t taille, const char *src) {
    if (!taille) return;
    snprintf(dest, taille, "%s", src ? src : "");
}

int comparerSansCasse(const char *a, const char *b) {
    while (*a && *b) {
        int diff = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (diff) return diff;
        a++; b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

int contientSansCasse(const char *texte, const char *mot) {
    if (!*mot) return 1;
    for (; *texte; texte++) {
        const char *a = texte, *b = mot;
        while (*a && *b && tolower((unsigned char)*a) == tolower((unsigned char)*b)) { a++; b++; }
        if (!*b) return 1;
    }
    return 0;
}

#include <limits.h>
#include <stdlib.h>

int agrandirTableau(void **tab, int *capacite, int minimum, size_t tailleElem) {
    if (*capacite >= minimum) return 1;
    int nouveau = *capacite > 0 ? *capacite : 2;
    while (nouveau < minimum) {
        if (nouveau > INT_MAX / 2) { nouveau = minimum; break; }
        nouveau *= 2;
    }
    if ((size_t)nouveau > ((size_t)-1) / tailleElem) return 0;
    size_t octets = (size_t)nouveau * tailleElem;
    void *temp;
    if (*tab == NULL) {
        temp = malloc(octets);
    } else {
        temp = realloc(*tab, octets);
    }
    if (!temp) return 0;
    *tab = temp;
    *capacite = nouveau;
    return 1;
}


int dateValide(Date d) {
    static const int jours[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (d.annee < 2000 || d.annee > 2100 || d.mois < 1 || d.mois > 12 || d.jour < 1) return 0;
    int max = jours[d.mois];
    if (d.mois == 2 && (d.annee % 400 == 0 || (d.annee % 4 == 0 && d.annee % 100 != 0))) max++;
    return d.jour <= max;
}


