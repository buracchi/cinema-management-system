#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/employee_management.h>
#include <cliutils/io.h>

#include "../core.h"

static char* get_role(char* role);

extern int insert_employee(cms_t cms) {
	struct cms_add_employee_request request = { 0 };
	struct cms_add_employee_response* response = NULL;
	io_clear_screen();
	puts(title);
	get_input_len("Nome: ", sizeof(request.name), (char*)request.name, false);
	get_input_len("Cognome: ", sizeof(request.surname), (char*)request.surname, false);
	get_role((char*)request.role);
	if (multi_choice("\nProcedere?", ((char[]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	puts("");
	try(cms_add_employee(cms, &request, &response), 1, fail);
	if (response->error_message) {
		printf("%s\n", response->error_message);
	}
	else {
		puts("Dipendente aggiunto con successo");
	}
	cms_destroy_response((struct cms_response*)response);
	press_anykey();
	return 0;
fail:
	if (response) {
		if (response->error_message) {
			fprintf(stderr, "%s\n", response->error_message);
		}
		cms_destroy_response((struct cms_response*)response);
	}
	return 1;
}
static char* get_role(char* role) {
	const char* roles[] = { "Maschera", "Proiezionista" };
	size_t selection = multi_choice("Ruolo <1=Maschera | 2=Proiezionista> ", ((char[]){ '1', '2' })) - '1';
	strcpy(role, roles[selection]);
	return role;
}
