#!/bin/bash
gcc -o webserver ./server/simple_server.c ./include/error_functions.c ./include/log.c ./include/util.c ./include/core.c ./include/http.c ./include/inet_sockets.c ./include/ip.c
./webserver