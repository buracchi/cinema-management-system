#include <buracchi/common/utilities/try.h>
#include <cms/cms.h>
#include <cms/employee_management.h>
#include <cliutils/io.h>

#include "../core.h"

static char* get_role(char* role);

extern int insert_employee(cms_t cms) {
	struct cms_response response;
	struct cms_employee_details employee_details = { 0 };
	io_clear_screen();
	puts(title);
	get_input("Nome: ", employee_details.name, false);
	get_input("Cognome: ", employee_details.surname, false);
	get_role((char*)employee_details.role);
	if (multi_choice("\nProcedere?", ((char[]){ 'S', 'N' })) == 'N') {
		return 0;
	}
	puts("");
	response = cms_add_employee(cms, &employee_details);
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
	puts("Dipendente aggiunto con successo");
	cms_destroy_response(&response);
	press_anykey();
	return 0;
}
static char* get_role(char* role) {
	const char* roles[] = { "Maschera", "Proiezionista" };
	size_t selection = multi_choice("Ruolo <1=Maschera | 2=Proiezionista> ", ((char[]){ '1', '2' })) - '1';
	strcpy(role, roles[selection]);
	return role;
}
