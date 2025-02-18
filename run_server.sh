#!/bin/bash

set -e

cd "$(dirname "$0")"

if [ ! -f ./include/ename.c.inc ]; then
    echo "Building ename.c.inc..."
    ./include/Build_ename.sh > ./include/ename.c.inc
fi

echo "Compiling the web server..."
gcc -o webserver ./server/simple_server.c ./include/error_functions.c ./include/log.c ./include/util.c ./include/core.c ./include/http.c ./include/inet_sockets.c ./include/ip.c

if [ $? -eq 0 ]; then
    echo "Compilation Successful. Running webserver..."
    ./webserver
else
    echo "Compilation Failed."
fi