#include <czmq.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Port number and ip are mandatory\n");
        return 0;
    }

    printf("Connecting to echo...\n");
    zsock_t *req = zsock_new(ZMQ_REQ);
    zsock_set_identity(req, "#0x01");
    zsock_connect(req, "tcp://localhost:%s", argv[1]);

    while (!zsys_interrupted) {
        char str[100];
        printf("Enter command test : \n");
        fgets(str, 100, stdin);
        zstr_sendf(req, str);
        char *message = zstr_recv(req);
        printf("Received : %s\n", message);
        zstr_free(&message);
    }
    zsock_destroy(&req);
    return 0;
}
