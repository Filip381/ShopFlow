SHOPFLOW - VERSION CLAIRE POUR DEBUTANT / PROJET XTI101
=======================================================

Le projet gere les produits, clients, commandes et statistiques, avec
des tableaux dynamiques et des donnees de demonstration. Les fichiers
sont separes selon LEUR ROLE. Chaque header de module annonce les fonctions presentes dans
le fichier .c de meme nom. main.c contient le point d'entree et les menus.

MODULES - FAIRE CORRESPONDRE .h ET .c
-------------------------------------
types.h         -> definit les 7 structures partagees (aucune implementation)
produit.h/.c    -> catalogue, recherche, stock
client.h/.c     -> coordonnees, adresses, historique, fidelite
commande.h/.c   -> creation, lignes, facturation, statuts, annulation
statistique.h/.c-> chiffres d'affaires, classements, ventes par categorie
initialisation.h/.c -> cree les exemples; libere les donnees a la fin
utils.h/.c     -> saisie securisee, validation, dates, textes et tableaux dynamiques
main.c          -> menus et demarrage du programme

FONCTIONS SUPPLEMENTAIRES (Section 8 du sujet)
----------------------------------------------
statistiques par categorie : menu statistiques > 4

FONCTIONNEMENT
--------------
Le programme utilise des tableaux dynamiques (malloc/realloc/free) et ne
sauvegarde aucune donnee sur disque. Au redemarrage, les donnees initiales
sont chargees a nouveau : 8 produits, 5 clients et 6 commandes.

COMPILER ET LANCER AVEC GCC
-------------------------
1. Decompresser le ZIP.
2. Ouvrir un terminal dans le dossier qui contient main.c.
3. Verifier que GCC est disponible avec : gcc --version
4. Compiler tous les fichiers .c ensemble :

gcc -std=c11 -Wall -Wextra -Wpedantic main.c produit.c client.c commande.c statistique.c initialisation.c utils.c -o shopflow

5. Lancer le programme :
   Windows (PowerShell) : .\shopflow.exe
   Linux ou macOS : ./shopflow

Pour essayer : choisir 1 puis 4 pour voir le catalogue. Revenir avec 0,
puis choisir 3, 2 et 1001 pour afficher la facture de demonstration.



