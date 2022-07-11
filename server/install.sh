#!/bin/sh

CREATE_USERS_SQL=$(cat <<EOF
CREATE USER IF NOT EXISTS 'amministratore'@'localhost'
    IDENTIFIED WITH mysql_native_password BY 'pippo';
CREATE USER IF NOT EXISTS 'cliente'@'localhost'
    IDENTIFIED WITH mysql_native_password BY 'pippo';
CREATE USER IF NOT EXISTS 'maschera'@'localhost'
    IDENTIFIED WITH mysql_native_password BY 'pippo';
EOF
)

echo "\n[+] Installing payment service plugin\n"
(
    BUILD_TYPE=release
    PRESET=x64-linux-$BUILD_TYPE
    BUILD_DIR=./out/build/x64-linux-$BUILD_TYPE
    MYSQL_PLUGIN_DIR=$(mysql -p -e "SHOW VARIABLES LIKE 'plugin_dir';" | grep -o "/.*")
    cd ./payment-service-plugin
    cmake --preset $PRESET
    cmake --build --preset $PRESET -j $(nproc)
    LIBRARY_PATH=$(readlink -f $BUILD_DIR/libpayment-service.so)
    cp $LIBRARY_PATH $MYSQL_PLUGIN_DIR/libpayment-service.so
)
echo "\n[+] Plugin installed\n"
echo "\n[+] Installing cinemadb database\n"
(
    cd ./cinemadb
    python3 make_sql_script.py
    echo -e "$CREATE_USERS_SQL $(cat build/cinemadb.sql)" | mysql -p
)
echo "\n[+] Database installed\n"
echo "Installation successfull."
