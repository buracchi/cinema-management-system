#include <payment-service.h>

#include <cstdlib>
#include <cstring>
#include <chrono>
#include <mutex>

static std::string mock_api_execute_transaction(std::string_view price,
	std::string_view name_on_card,
	std::string_view card_number,
	std::string_view expiration_date,
	std::string_view cvv2);

static std::mutex mock_api_mutex;

extern bool EFFETTUA_PAGAMENTO_init(UDF_INIT* initid, const UDF_ARGS* args, char* message) {
	if (args->arg_count != 5
		|| args->arg_type[0] != DECIMAL_RESULT
		|| args->arg_type[1] != STRING_RESULT
		|| args->arg_type[2] != DECIMAL_RESULT
		|| args->arg_type[3] != STRING_RESULT
		|| args->arg_type[4] != DECIMAL_RESULT) {
		std::strcpy(message, "Segnatura richiesta: EFFETTUA_PAGAMENTO(decimal, string, decimal, data, decimal).");
		return true;
	}
	initid->const_item = false;
	initid->maybe_null = true;
	initid->max_length = 256;
	initid->ptr = (char*)malloc(initid->max_length);
	if (!initid->ptr) {
		std::strcpy(message, "Memoria insufficiente per eseguire EFFETTUA_PAGAMENTO().");
		return true;
	}
	return false;
}

extern char* EFFETTUA_PAGAMENTO(UDF_INIT* initid, const UDF_ARGS* args, const char*, unsigned long* length, char* is_null, char* error) {
	std::string price(args->args[0], args->lengths[0]);
	std::string name_on_card(args->args[1], args->lengths[1]);
	std::string card_number(args->args[2], args->lengths[2]);
	std::string expiration_date(args->args[3], args->lengths[3]);
	std::string cvv2(args->args[4], args->lengths[4]);
	std::string transaction_id = mock_api_execute_transaction(price, name_on_card, card_number, expiration_date, cvv2);
	*length = (unsigned long)transaction_id.length();
	*is_null = false;
	*error = false;
	std::memcpy(initid->ptr, transaction_id.c_str(), transaction_id.length());
	return initid->ptr;
}

extern void EFFETTUA_PAGAMENTO_deinit(UDF_INIT* initid) {
	free(initid->ptr);
}

extern bool EFFETTUA_RIMBORSO_init(UDF_INIT* initid, const UDF_ARGS* args, char* message) {
	if (args->arg_count != 1 || args->arg_type[0] != STRING_RESULT) {
		std::strcpy(message, "EFFETTUA_RIMBORSO() richiede 1 parametro stringa.");
		return true;
	}
	initid->const_item = true;
	initid->maybe_null = false;
	return false;
}

extern long long EFFETTUA_RIMBORSO(const UDF_INIT*, const UDF_ARGS*, const char*, const char*) {
	return 0;
}

extern void EFFETTUA_RIMBORSO_deinit(const UDF_INIT*) {
	// this empty function is necessary to MySQL if the --allow-suspicious-udfs option is not set
}

static std::string mock_api_execute_transaction(
	[[maybe_unused]] std::string_view price,
	[[maybe_unused]] std::string_view name_on_card,
	[[maybe_unused]] std::string_view card_number,
	[[maybe_unused]] std::string_view expiration_date,
	[[maybe_unused]] std::string_view cvv2) {
	std::scoped_lock<std::mutex> lock(mock_api_mutex);
	auto now = std::chrono::system_clock::now();
	auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
	return std::to_string(timestamp);
}
