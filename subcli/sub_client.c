#include <czmq.h>

int main(int argc, char *argv[])
{
  if (argc < 4) {
    printf("Port number, ip and channel are mandatory\n");
    return 0;
  }

  zsock_t *client = zsock_new(ZMQ_SUB);
  zsock_connect(client, "tcp://%s:%s", argv[1], argv[2]);
  printf("Client connected to %s:%s\n", argv[1], argv[2]);

  zsock_set_subscribe(client, argv[3]);
  zsock_set_subscribe(client, "#general:");

  while (!zsys_interrupted) {
    char *message = zstr_recv(client);
    printf("%s", message);
    zstr_free(&message);
  }

  zsock_destroy(&client);
  return 0;
}
