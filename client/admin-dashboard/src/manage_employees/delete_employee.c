#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/employee_management.h>
#include <cliutils/io.h>

#include "../core.h"

extern int select_employee(cms_t cms, struct cms_employee* selected_employee);

extern int delete_employee(cms_t cms) {
	struct cms_response response;
	struct cms_employee employee;
	switch (select_employee(cms, &employee)) {
	case 1:
		goto fail;
	case 2:
		return 0;
	}
	io_clear_screen();
	puts(title);
	printf("Dipendente: %d - %s %s\n\n", employee.id, employee.name, employee.surname);
	if (multi_choice("Il dipendente selezionato verra' rimosso, procedere?", ((char[]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	puts("");
	response = cms_delete_employee(cms, employee.id);
	if (response.fatal_error) {
		fprintf(stderr, "%s\n", response.error_message ? response.error_message : cms_get_error_message(cms));
		cms_destroy_response(&response);
		return 1;
	}
	if (response.error_message) {
		printf("%s\n", response.error_message);
		cms_destroy_response(&response);
		press_anykey();
		return 0;
	}
	puts("Dipendente rimosso con successo");
	cms_destroy_response(&response);
	press_anykey();
	return 0;
fail:
	return 1;
}
