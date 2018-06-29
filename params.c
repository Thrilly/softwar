//
//  main.c
//  softwar
//
//  Created by Brice Fouepe on 23/06/2018.
//  Copyright © 2018 Brice Fouepe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Prototype de mes fonctions de controle d'erreurs

int find_mode(const char * str);
int find_size(const char * str);
int find_log(const char * str);
int find_cycle(const char * str);
int find_rep_port(const char * str);
int find_pub_port(const char * str);

int main(int argc, const char * argv[]) {
    time_t t;
    int i;
    int cycle = 0;
    int pos_mode = -1;
    int pos_size = -1;
    int pos_log = -1;
    int pos_cycle = -1;
    int pos_rep_port = -1;
    int pos_pub_port = -1;

    //On vérifie le nombre d'arguments passés au programme

    if(argc !=6 && argc != 7){
        printf("Erreur: nombre d'arguments incorrect\n");
        return(-1);
    }

    for (i=0; i<argc; i++) {
        if (find_mode(argv[i])) {
            pos_mode = i;
        }

        if (find_log(argv[i])){
            pos_log = i;
        }

        if (find_size(argv[i])){
            pos_size = i;
        }

        if (find_cycle(argv[i])){
            pos_cycle = i;
        }

        if (find_rep_port(argv[i])){
            pos_rep_port = i;
        }

        if (find_pub_port(argv[i])){
            pos_pub_port = i;
        }
    }

// On check les erreurs si le nombre d'arguments est 7

    if ( argc == 7) {
        if (pos_mode == -1) {
            printf("Erreur: l'argument mode n'a pas le bon format\n");
            return -1;
        }

        if (pos_size == -1) {
            printf("Erreur: l'argument size n'a pas le bon format\n");
            return -1;
        }

        if (pos_log == -1) {
            printf("Erreur: l'argument log n'a pas le bon format\n");
            return -1;
        }

        if (pos_cycle == -1) {
            printf("Erreur: l'argument cycle n'a pas le bon format\n");
            return -1;
        }

        if (pos_rep_port == -1) {
            printf("Erreur: l'argument rep-port n'a pas le bon format\n");
            return -1;
        }

        if (pos_pub_port == -1) {
            printf("Erreur: l'argument pub-port n'a pas le bon format\n");
            return -1;
        }
    }

    //On check les erreurs si le nombre d'arguments est 6 et le cycle n'est pas compris

    if (argc == 6 && ((pos_mode == -1 && pos_log != -1 && pos_size != -1 && pos_cycle != -1 && pos_pub_port != -1 && pos_rep_port != -1)

        || (pos_mode != -1 && pos_log == -1 && pos_size != -1 && pos_cycle != -1 && pos_pub_port != -1 && pos_rep_port != -1)

        || (pos_mode != -1 && pos_log != -1 && pos_size == -1 && pos_cycle != -1 && pos_pub_port != -1 && pos_rep_port != -1)

        || (pos_mode != -1 && pos_log != -1 && pos_size != -1 && pos_cycle == -1 && pos_pub_port != -1 && pos_rep_port != -1)

        || (pos_mode != -1 && pos_log != -1 && pos_size != -1 && pos_cycle != -1 && pos_pub_port == -1 && pos_rep_port != -1)

        || (pos_mode != -1 && pos_log != -1 && pos_size != -1 && pos_cycle != -1 && pos_pub_port != -1 && pos_rep_port == -1))) {

        if(pos_mode == -1){
            printf("Erreur: l'argument mode n'a pas le bon format\n");
            return -1;
        }

        if(pos_pub_port == -1){
            printf("Erreur: l'argument pub-port n'a pas le bon format\n");
            return -1;
        }

        if(pos_rep_port == -1){
            printf("Erreur: l'argument rep-port n'a pas le bon format\n");
            return -1;
        }

        if(pos_size == -1){
            printf("Erreur: l'argument cycle n'a pas le bon format\n");
            return -1;
        }

        if(pos_log == -1){
            printf("Erreur: l'argument log n'a pas le bon format\n");
            return -1;
        }

    }

    //On check si la size est comprise entre 0 et 5

    if (atoi(&argv[pos_size][7]) < 0 || atoi(&argv[pos_size][7]) > 5) {
        printf("Erreur: Vous devez insérer une size comprise entre 0 et 5\n");
        return -1;
    }

    // On génère un nombre random si le nombre d'arguments est 6 et s'il manque la variable cycle

    if (argc == 6){
        srand((unsigned) &t);
        cycle=rand();
    }

    printf("%d\n", cycle);

    return 0;
}

/* ------ Differentes fonctions pour la gestion des erreurs ------- */

int find_mode(const char *str){
    if(strcmp(str, "-v") == 0){
        return 1;
    }
    else
        return 0;
}
int find_size(const char *str){
    if(strncmp(str, "--size", 6) == 0){
        return 1;
    }
    else
        return 0;
}

int find_log(const char *str){
    if(strncmp(str, "--log", 5) == 0){
        return 1;
    }
    else
        return 0;
}

int find_cycle(const char *str){
    if(strncmp(str, "--cycle", 7) == 0){
        return 1;
    }
    else
        return 0;
}

int find_rep_port(const char *str){
    if(strcmp(str, "--rep-port 4242") == 0){
        return 1;
    }
    else
        return 0;
}

int find_pub_port(const char *str){
    if(strcmp(str, "--pub-port 4243") == 0){
        return 1;
    }
    else
        return 0;
}
