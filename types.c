/* types.c - Fonctions associees a la structure Date de types.h. */
#include "types.h"
#include <time.h>

int dateValide(Date d) {
    static const int jours[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (d.annee < 2000 || d.annee > 2100 || d.mois < 1 || d.mois > 12 || d.jour < 1) return 0;
    int max = jours[d.mois];
    if (d.mois == 2 && (d.annee % 400 == 0 || (d.annee % 4 == 0 && d.annee % 100 != 0))) max++;
    return d.jour <= max;
}

Date dateAujourdhui(void) {
    time_t maintenant = time(NULL);
    struct tm *tm = localtime(&maintenant);
    Date d = {1, 1, 2026};
    if (tm && tm->tm_year + 1900 >= 2000 && tm->tm_year + 1900 <= 2100)
        d = (Date){tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900};
    return d;
}
