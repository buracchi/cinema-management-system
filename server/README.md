# cinema-management-system server

## payment-service-plugin installation

Il will be necessary to specify somehow the folder in which MYSQL was installed, maybe as a CMAKE flag, to install the dynamic library in the MySQL plugins folder.

## payment-service-plugin cross-platform issues

To implement the payment service MySQL 8 require to declare the dynamic library name inside the `CREATE FUNCTION` statement, if the server will not be hosted on a Windows based OS it will be required to swap the instances of `payment-service-plugin.dll` with `payment-service-plugin.so` inside the `cinemadb.sql` file if used to instantiate the database or inside the `cinemadb.mwb` file with a tool such as MySQL Workbench.
