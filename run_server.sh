#!/bin/bash

set -e

cd "$(dirname "$0")"

if [ ! -f ./include/ename.c.inc ]; then
    echo "Building ename.c.inc..."
    ./include/Build_ename.sh > ./include/ename.c.inc
fi

echo "Select the server type:"
echo "1) Threading Server"
echo "2) Forking Server"
read -p "Enter choice (1 or 2): " choice

if [ "$choice" == "1" ]; then
    SERVER_TYPE="threading_server"
elif [ "$choice" == "2" ]; then
    SERVER_TYPE="forking_server"
else
    echo "Invalid choice. Exiting."
    exit 1
fi

echo "Compiling the $SERVER_TYPE..."
gcc -o webserver ./server/${SERVER_TYPE}.c ./include/error_functions.c ./include/log.c ./include/util.c ./include/core.c ./include/http.c ./include/inet_sockets.c ./include/ip.c

if [ $? -eq 0 ]; then
    echo "Compilation Successful. Running webserver..."
    ./webserver
else
    echo "Compilation Failed."
fi