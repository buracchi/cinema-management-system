#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/employee_management.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_employee(cms_t cms, struct cms_employee* employee);

extern int delete_employee(cms_t cms) {
	struct cms_delete_employee_request request = { 0 };
	struct cms_delete_employee_response* response;
	struct cms_employee employee;
	switch (select_employee(cms, &employee)) {
	case 1:
		goto fail;
	case 2:
		return 0;
	};
	io_clear_screen();
	puts(title);
	printf("Dipendente: %d %s %s\n\n", employee.id, employee.name, employee.surname);
	if (multi_choice("Il dipendente selezionato verra' rimosso, procedere?", ((char[2]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	request.id = employee.id;
	try(cms_delete_employee(cms, request, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
	}
	else {
		puts("\nDipendente rimosso con successo");
	}
	cms_destroy_response((struct cms_response*)response);
	press_anykey();
	return 0;
fail:
	return 1;
}
