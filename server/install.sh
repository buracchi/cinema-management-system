#!/bin/sh

BUILD_TYPE=release
PRESET=x64-linux-$BUILD_TYPE
BUILD_DIR=./out/build/x64-linux-$BUILD_TYPE
MYSQL_PLUGIN_DIR=$(mysql -e "SHOW VARIABLES LIKE 'plugin_dir';" | grep -o "/.*")

echo "\n[+] Installing cinemadb database\n"
(
    cd ./cinemadb
    python3 make_sql_script.py
    mysql < build/cinemadb.sql
)
echo "\n[+] Database installed\n"
echo "\n[+] Installing plugin\n"
(
    cd ./payment-service-plugin
    cmake --preset $PRESET
    cmake --build --preset $PRESET -j $(nproc)
    mv $BUILD_DIR/libpayment-service.so $MYSQL_PLUGIN_DIR
)
echo "\n[+] Plugin installed\n"
echo "Installation successfull."
