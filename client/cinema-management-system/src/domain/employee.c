#include "domain/employee.h"

struct employee {
	int32_t id;
	char* name;
	char* surname;
	enum role role;
};

extern int32_t employee_get_id(employee_t employee) {
	return employee->id;
}

extern char* employee_get_name(employee_t employee) {
	return employee->name;
}

extern char* employee_get_surname(employee_t employee) {
	return employee->surname;
}

extern enum role employee_get_role(employee_t employee) {
	return employee->role;
}
