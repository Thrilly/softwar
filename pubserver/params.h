
char REQPORT[]  = "4242";
char PUBPORT[]  = "4243";
char CYCLE[]    = "1000000";
char MAPSIZE[]  = "5";
char LOGS[]     = "/tmp/soft_war.log";
int VERBOSE     = 0;

int check_params(int argc, char *argv[])
{
    for (int i = 1; i < argc; i= i+2) {

        // *********** PUB PORT ***********
        if (strcmp(argv[i],"--pub-port") == 0) {
            if (argv[i+1] != NULL) { // control if params value exist
                if (atoi(argv[i+1]) > 0 && atoi(argv[i+1]) < 65535) { // control data validity
                    strcpy(PUBPORT, argv[i+1]); // assing params
                } else { // else print error
                    printfc("[FAIL] - Unable runing server : Invalid port, must be integer beetween 1 and 65535\n", "red");
                    return 0;
                }
            } else { // else print error
                printfc("[FAIL] - PUB Port number is expected after --pub-port parameter\n", "red");
                return 0;
            }
        // *********** END PUB PORT ***********

        // *********** REQ PORT ***********
        } else if (strcmp(argv[i],"--req-port") == 0) {
            if (argv[i+1] != NULL) { // control if params value exist
                if (atoi(argv[i+1]) > 0 && atoi(argv[i+1]) < 65535) { // control data validity
                    strcpy(REQPORT, argv[i+1]); // assing params
                } else { // else print error
                    printfc("[FAIL] - Unable runing server : Invalid port, must be integer beetween 1 and 65535\n", "red");
                    return 0;
                }
            } else { // else print error
                printfc("[FAIL] - REQ Port number is expected after --req-port parameter\n", "red");
                return 0;
            }
        // *********** END REQ PORT ***********


        // *********** CYCLE ***********
        } else if(strcmp(argv[i],"--cycle") == 0) {
            if (argv[i+1] != NULL) { // control if params value exist
                if (atoi(argv[i+1]) >= 100000 && atoi(argv[i+1]) <= 5000000) { // control data validity
                    strcpy(CYCLE, argv[i+1]); // assing params
                } else { // else print error
                    printfc("[FAIL] - Unable runing server : Cannot set this cycle interval, must be integer beetween 100000 and 5000000\n", "red");
                    return 0;
                }
            } else { // else print error
                printfc("[FAIL] - Cycle ms interval is expected after --cycle parameter\n", "red");
                return 0;
            }
        // *********** END CYCLE ***********


        // *********** MAPSIZE ***********
        } else if(strcmp(argv[i],"--size") == 0) {
            if (argv[i+1] != NULL) { // control if params value exist
                if (atoi(argv[i+1]) >= 5 && atoi(argv[i+1]) <= 20) { // control data validity
                    strcpy(MAPSIZE, argv[i+1]); // assing params
                } else { // else print error
                    printfc("[FAIL] - Unable runing server : Cannot set map size, it must be integer beetween 5 and 20\n", "red");
                    return 0;
                }
            } else { // else print error
                printfc("[FAIL] - Map size value is expected after --size parameter\n", "red");
                return 0;
            }
        // *********** END MAPSIZE ***********


        // *********** LOGS ***********
        } else if(strcmp(argv[i],"--log") == 0) {
            if (argv[i+1] != NULL) {
                strcpy(LOGS, argv[i+1]); // assing params
            } else { // else print error
                printfc("[FAIL] - Log path is expected after --log parameter\n", "red");
                return 0;
            }
        // *********** END LOGS ***********


        // *********** VERBOSE ***********
        } else if(strcmp(argv[i],"-v") == 0) {
            VERBOSE = 1; // assing params
            i--; // remove 1 to i because -v don't expect parameters
        // *********** END VERBOSE ***********


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

    // Test logs
    FILE *f;
    f = fopen(LOGS, "a");
    if (f == NULL) {
        printfc("[FAIL] - Cannot oppening ", "red");
        printfc(LOGS, "blue");
        printfc(". Unknow directory \n", "red");
        return 0;
    }
    fclose(f);
    char logsStr[255];
    logs(LOGS, "server", "Server starting instance", "INFO");
    sprintf(logsStr, "Server params settings (req_port:%s,pubport:%s,cycle:%s,map_size:%s)", REQPORT, PUBPORT, CYCLE, MAPSIZE);
    logs(LOGS, "server", logsStr, "INFO");

    if (VERBOSE == 1) {
        printf("[INFO] - Verbose mode actived \n");
        usleep(100000);
        printf("[INFO] - Serveur socket ROUTER listenning on tcp://*:%s \n", REQPORT);
        usleep(100000);
        if (atoi(REQPORT) < 1000) {
            printfc("[WARN] - It\'s recommanded to use port higher than 1000 for socket ROUTER\n", "yellow");
            logs(LOGS, "server", "Socket port TCP setted under 1000 value", "WARN");
            usleep(100000);
        }
        printf("[INFO] - Serveur socket PUB listenning on tcp://*:%s \n", PUBPORT);
        usleep(100000);
        if (atoi(PUBPORT) < 1000) {
            printfc("[WARN] - It\'s recommanded to use port higher than 1000 for socket PUB\n", "yellow");
            logs(LOGS, "server", "Socket port TCP setted under 1000 value", "WARN");
            usleep(100000);
        }
        printf("[INFO] - Serveur notify by %s microsecondes cycle \n", CYCLE);
        usleep(100000);
        printf("[INFO] - Map size setted on %s \n", MAPSIZE);
        usleep(100000);
        printf("[INFO] - Logs writing in %s \n", LOGS);
        usleep(100000);
    }

    printfc("[SUCC] - Server is ready ! Starting server \n", "green");
    for (int i = 0; i < WAIT_BEFORE_START_SERVER; i++) {
        printfc(".\n", "green");
        sleep(1);
    }
    printf("\n");
    return 1;
}
