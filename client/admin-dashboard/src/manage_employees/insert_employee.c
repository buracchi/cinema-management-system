#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/employee_management.h>
#include <cliutils/io.h>

#include "../core.h"

extern int insert_employee(cms_t cms) {
	struct cms_add_employee_request request = { 0 };
	struct cms_add_employee_response* response;
	io_clear_screen();
	puts(title);
	get_input_len("Nome: ", sizeof(request.name), (char*)request.name, false);
	get_input_len("Cognome: ", sizeof(request.surname), (char*)request.surname, false);
	get_input_len("Ruolo <Maschera | Proiezionista>: ", sizeof(request.role), (char*)request.role, false);
	if (multi_choice("Procedere?", ((char[2]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	try(cms_add_employee(cms, &request, &response), 1, fail);
	if (response->error_message) {
		printf("%s", response->error_message);
	}
	else {
		puts("\nDipendente aggiunto con successo");
	}
	cms_destroy_response((struct cms_response*)response);
	press_anykey();
	return 0;
fail:
	return 1;
}
