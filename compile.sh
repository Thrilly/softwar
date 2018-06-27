#!/usr/bin/env bash
gcc -Wall -o bin/server files/pub_server.c -l czmq -l zmq
gcc -Wall -o bin/cli files/sub_client.c -l czmq -l zmq
