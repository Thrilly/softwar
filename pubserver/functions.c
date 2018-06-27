//  check_params.c
//  softwar
//
//  Created by Brice Fouepe on 23/06/2018.
//  Copyright © 2018 Brice Fouepe. All rights reserved.
//

int check_params(int argc, char *argv[])
{
    if(argc % 2 != 0){
        for (int i = 1; i < argc; i= i+2) {

            // *********** PUB PORT ***********
            if (strcmp(argv[i],"-pub-port") == 0) {
                if (atoi(argv[i+1]) > 0 && atoi(argv[i+1]) < 65535) { // control data validity
                    strcpy(PUBPORT, argv[i+1]); // assing params
                    // printf("[INFO] - Serveur listenning on tcp://*:%s \n", PUBPORT);
                } else { // else print error
                    printf("[ERROR] - Unable runing server : Invalid port, must be beetween 1 and 65535\n");
                    return 0;
                }
            // *********** CYCLE ***********
            } else if(strcmp(argv[i],"-cycle") == 0) {
                if (atoi(argv[i+1]) > 0 && atoi(argv[i+1]) <= 5000000) { // control data validity
                    strcpy(CYCLE, argv[i+1]); // assing params
                    // printf("[INFO] - Serveur notify by %s microsecondes cycle \n", CYCLE);
                } else { // else print error
                    printf("[ERROR] - Unable runing server : Cannot set this cycle interval, must be beetween 100000 and 5000000\n");
                    return 0;
                }
            // *********** INVALID PARAMETER ***********
            } else {
                printf("[ERROR] - Unable runing server : Unknow parameter %s\n", argv[i]);
                return 0;
            }
        }
    // *********** INVALID PARAMETER NUMBER ***********
    } else {
        printf("[ERROR] - Unable runing server : Invalid number of parameters \n");
        return 0;
    }
    printf("[INFO] - Server is running ...\n");
    return 1;
}

void gameinfo_to_json(Gameinfo *gameinfo)
{
    printf("{\"map_size\": %s, \"game_status\" : %s}\n", gameinfo->map_size, gameinfo->game_status);
}
