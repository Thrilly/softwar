
char PUBPORT[]  = "4243";
char CYCLE[]    = "1000000";

int check_params(int argc, char *argv[])
{
    for (int i = 1; i < argc; i= i+2) {
        // *********** PUB PORT ***********

        if (strcmp(argv[i],"-pub-port") == 0) {
            if (argv[i+1] != NULL) {
                if (atoi(argv[i+1]) > 0 && atoi(argv[i+1]) < 65535) { // control data validity
                    if (atoi(argv[i+1]) < 1000) {
                        printfc("[WARN] - It\'s recommanded to use port higher than 1000\n", "yellow");
                    }
                    strcpy(PUBPORT, argv[i+1]); // assing params
                } else { // else print error
                    printfc("[FAIL] - Unable runing server : Invalid port, must be integer beetween 1 and 65535\n", "red");
                    return 0;
                }
            } else {
                printfc("[FAIL] - Port number is expected after -pub-port parameter\n", "red");
                return 0;
            }
        // *********** END PUB PORT ***********

        // *********** CYCLE ***********
        } else if(strcmp(argv[i],"-cycle") == 0) {
            if (argv[i+1] != NULL) {
                if (atoi(argv[i+1]) > 0 && atoi(argv[i+1]) <= 5000000) { // control data validity
                    strcpy(CYCLE, argv[i+1]); // assing params
                } else { // else print error
                    printfc("[FAIL] - Unable runing server : Cannot set this cycle interval, must be integer beetween 100000 and 5000000\n", "red");
                    return 0;
                }
            } else {
                printfc("[FAIL] - Cycle ms interval is expected after -cycle parameter\n", "red");
                return 0;
            }
        // *********** END CYCLE ***********

        // *********** INVALID PARAMETER ***********
        } else {
            printfc("[FAIL] - Unable running server : Unknow parameter ", "red");
            printfc(argv[i], "blue");
            printf("\n");
            return 0;
        }
        // *********** END INVALID PARAMETER ***********
    }

    // *********** PRINT INFOS ***********
    printf("[INFO] - Serveur listenning on tcp://*:%s \n", PUBPORT);
    printf("[INFO] - Serveur notify by %s microsecondes cycle \n", CYCLE);
    printfc("[SUCC] - Server is ready ! Start in 3 sec\n", "green");
    sleep(3);
    return 1;
}
