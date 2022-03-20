#pragma once

#include <mysql.h>
#include <buracchi/common/containers/map.h>

struct cinema_management_service {
	MYSQL* db_connection;
	cmn_map_t mysql_statement_data_map;
};
