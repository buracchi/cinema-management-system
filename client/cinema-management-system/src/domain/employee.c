#include "domain/employee.h"

struct employee {
	int32_t id;
	char* name;
	char* surname;
	enum role role;
};

extern int32_t get_id(employee_t employee) {
	return employee->id;
}

extern char* get_name(employee_t employee) {
	return employee->name;
}

extern char* get_surname(employee_t employee) {
	return employee->surname;
}

extern enum role get_role(employee_t employee) {
	return employee->role;
}
