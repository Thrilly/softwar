//  check_params.c
//  softwar
//
//  Created by Brice Fouepe on 23/06/2018.
//  Copyright © 2018 Brice Fouepe. All rights reserved.
//

#include <czmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_params(int argc, char *argv[])
{
    for (int i = 1; i < argc; i= i+2) {

        // *********** PUB PORT ***********
        if (strcmp(argv[i],"-pub-port") == 0) {
            if (atoi(argv[i+1]) > 0 && atoi(argv[i+1]) < 65535) { // control data validity
                if (atoi(argv[i+1]) < 1000) {
                    printfc("[WARN] - It\'s recommanded to use port higher than 1000\n", "yellow");
                }
                strcpy(PUBPORT, argv[i+1]); // assing params
            } else { // else print error
                printfc("[FAIL] - Unable runing server : Invalid port, must be beetween 1 and 65535\n", "red");
                return 0;
            }
        // *********** END PUB PORT ***********

        // *********** CYCLE ***********
        } else if(strcmp(argv[i],"-cycle") == 0) {
            if (atoi(argv[i+1]) > 0 && atoi(argv[i+1]) <= 5000000) { // control data validity
                strcpy(CYCLE, argv[i+1]); // assing params
            } else { // else print error
                printfc("[FAIL] - Unable runing server : Cannot set this cycle interval, must be beetween 100000 and 5000000\n", "red");
                return 0;
            }
        // *********** END CYCLE ***********

        // *********** INVALID PARAMETER ***********
        } else {
            printfc("[FAIL] - Unable runing server : Unknow parameter ", "red");
            printfc(argv[i], "red");
            printf("\n");
            return 0;
        }
        // *********** END INVALID PARAMETER ***********
    }

    return 1;
}

void printfc(char* str, char* color)
{
    if (strcmp(color,"green") == 0) {
        printf("\x1B[32m%s", str);
    } else if(strcmp(color,"red") == 0){
        printf("\x1B[31m%s", str);
    } else if(strcmp(color,"blue") == 0){
        printf("\x1B[36m%s", str);
    } else if(strcmp(color,"yellow") == 0){
        printf("\x1B[33m%s", str);
    }
    printf("\x1B[0m");
}

char* gameinfo_to_json(GameInfo gameinfo)
{
    char* json = malloc(sizeof (char) * 1024);
    sprintf(json, "#general: {\"map_size\": %i, \"game_status\" : %i}\n", gameinfo.map_size, gameinfo.game_status);
    return json;
}

void print_server_state(int status, int loop, int pubPort, int cycle, int nbConn, char* msg){
    printf("\e[1;1H\e[2J");
    printf("########## SOFTWAR SERVER V1.0 ##########\n\n");
    printf("STATUS     | ");
    if (status == 1) { printfc("RUNNING\n", "green"); }
    else if (status == 2) { printfc("PAUSE\n", "yellow"); }
    else if (status == 3) { printfc("STOPPED\n", "red"); }
    printf("PKT SENDED | %i\n", loop);
    printf("PUB PORT   | %i\n", pubPort);
    printf("CYCLE      | %i\n", cycle);
    printf("NB CONN    | %i\n", nbConn);
    printf("MSG        | ");
    printfc(msg, "blue");
    printf("\n");
    printf("##########################################\n");
}
