#pragma once
/*
* Mock of UDF (user definable functions) that are dynamicly loaded into
* the standard mysqld core.
*
* The functions name, type and shared library is saved in the new system
* table 'func'.  To be able to create new functions one must have write
* privilege for the database 'mysql'.	If one starts MySQL with
* --skip-grant-tables, then UDF initialization will also be skipped.
*
* Syntax for the new commands are:
* create function <function_name> returns {string|real|integer}
* 	  soname <name_of_shared_library>
* drop function <function_name>
*
* Each defined function may have a xxxx_init function and a xxxx_deinit
* function.  The init function should alloc memory for the function
* and tell the main function about the max length of the result
* (for string functions), number of decimals (for double functions) and
* if the result may be a null value.
*
* If a function sets the 'error' argument to 1 the function will not be
* called anymore and mysqld will return NULL for all calls to this copy
* of the function.
*
* All strings arguments to functions are given as string pointer + length
* to allow handling of binary data.
* Remember that all functions must be thread safe. This means that one is not
* allowed to alloc any global or static variables that changes!
* If one needs memory one should alloc this in the init function and free
* this on the __deinit function.
*
* Note that the init and __deinit functions are only called once per
* SQL statement while the value function may be called many times
*
* A dynamicly loadable file should be compiled shared.
*
* After the library is made one must notify mysqld about the new
* functions.
*
* On Windows use the commands:
*
* CREATE FUNCTION EFFETTUA_PAGAMENTO RETURNS STRING SONAME "payment-service.dll";
* CREATE FUNCTION EFFETTUA_RIMBORSO RETURNS INTEGER SONAME "payment-service.dll";
*
* Otherwise use the commands:
*
* CREATE FUNCTION EFFETTUA_PAGAMENTO RETURNS STRING SONAME "libpayment-service.so";
* CREATE FUNCTION EFFETTUA_RIMBORSO RETURNS INTEGER SONAME "libpayment-service.so";
*
* After this the functions will work exactly like native MySQL functions.
* Functions should be created only once.
*
* The functions can be deleted by:
*
* DROP FUNCTION EFFETTUA_PAGAMENTO;
* DROP FUNCTION EFFETTUA_RIMBORSO;
*
* The CREATE FUNCTION and DROP FUNCTION update the func@mysql table. All
* Active function will be reloaded on every restart of server
* (if --skip-grant-tables is not given)
*
* If you ge problems with undefined symbols when loading the shared
* library, you should verify that mysqld is compiled with the -rdynamic
* option.
*
* If you can't get AGGREGATES to work, check that you have the column
* 'type' in the mysql.func table.  If not, run 'mysql_upgrade'.
*
*/

#if defined(_WIN32)
#  if defined(EXPORTING_PAYMENT_SERVICE_PLUGIN)
#    define DECLSPEC __declspec(dllexport)
#  else
#    define DECLSPEC __declspec(dllimport)
#  endif
#else // non windows
#  define DECLSPEC
#endif

#include <mysql.h>

#ifdef __cplusplus
extern "C" {
#endif

	DECLSPEC extern bool EFFETTUA_PAGAMENTO_init(UDF_INIT* initid, const UDF_ARGS* args, char* message);

	DECLSPEC extern char* EFFETTUA_PAGAMENTO(UDF_INIT* initid, const UDF_ARGS* args, const char*, unsigned long* length, char* is_null, char* error);

	DECLSPEC extern void EFFETTUA_PAGAMENTO_deinit(UDF_INIT* initid);

	DECLSPEC extern bool EFFETTUA_RIMBORSO_init(UDF_INIT* initid, const UDF_ARGS* args, char* message);

	DECLSPEC extern long long EFFETTUA_RIMBORSO(const UDF_INIT*, const UDF_ARGS*, const char*, const char*);

	DECLSPEC extern void EFFETTUA_RIMBORSO_deinit(const UDF_INIT*);

#ifdef __cplusplus
}
#endif
