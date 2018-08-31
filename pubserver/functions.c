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
#include "global.h"

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

void logs(char *path, char* author, char* message, char* type) {
    FILE *f;
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    if ((f = fopen(path,"a")) == NULL)
    {
        printf("ERROR FOPEN !\n");
    }
    fprintf(f, "[%s] %s - |%s| %s\n", type, asctime (timeinfo), author, message);
    fclose(f);
}

char* notify(int notificationType, GameInfo gameinfo, Player* list_players, EnergyCell* list_energy)
{
    char* json = malloc(sizeof (char) * 4096);
    char* players_json = get_player_list(list_players);
    char* energy_json = get_energy_list(list_energy);
    if (notificationType == 8) {
        // sprintf(json, "{\"notification_type\": %i, \"data\" : {\"players\": %s, \"energy_cells\" : %s}}\n", notificationType, players_json, energy_json);
        // sprintf(json, "{\"notification_type\": %i, \"data\" : {\"map_size\": %i, \"game_status\" : %i}}\n", notificationType, gameinfo.map_size, gameinfo.game_status);
    } else {
        sprintf(json, "{\"notification_type\": %i, \"data\" : {\"map_size\": %i, \"game_status\": %i, \"players\": %s, \"energy_cells\" : %s}}\n", notificationType, gameinfo.map_size, gameinfo.game_status, players_json, energy_json);
    }
    free(players_json);
    free(energy_json);
    return json;
}

void print_server_state(int status, int loop, int pubPort, int cycle, int nbConn, char* msg){
    printf("\e[1;1H\e[2J");
    printf("########## SOFTWAR SERVER V1.0 ##########\n\n");
    printf("STATUS     | ");
    if (status == -1) { printfc("STOPPED\n", "red"); }
    if (status == 0) { printfc("WAITING FOR PLAYERS\n", "yellow"); }
    else if (status == 1) { printfc("IN GAME\n", "green"); }
    else if (status == 2) { printfc("FINISHED\n", "red"); }
    printf("PKT SENDED | %i\n", loop);
    printf("PUB PORT   | %i\n", pubPort);
    printf("CYCLE      | %i ms\n", cycle);
    printf("NB CONN    | %i\n", nbConn);
    printf("MSG        | ");
    printfc(msg, "blue");
    printf("\n");
    printf("##########################################\n");
}

double time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;

    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;

    diff = (double)y_ms - (double)x_ms;

    return diff;
}

char *substr(char *src,int pos,int len) {
    char *dest=NULL;
    if (len>0) {
        dest = calloc(len+1, 1);
        if(NULL != dest) {
            strncat(dest,src+pos,len);
        }
    }
    return dest;
}
