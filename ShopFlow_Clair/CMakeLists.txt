cmake_minimum_required(VERSION 4.3)
project(ShopFlow_Clair C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ShopFlow_Clair
        client.c
        client.h
        commande.c
        commande.h
        initialisation.c
        initialisation.h
        main.c
        produit.c
        produit.h
        statistique.c
        statistique.h
        types.h
        utils.c
        utils.h)
