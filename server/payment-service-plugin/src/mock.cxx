#include <payment-service.h>
#include <cstdlib>
#include <chrono>
#include <mutex>
#include <future>

static std::string mock_api_pay(std::string_view price, std::string_view name_on_card, long long card_number, std::string_view expiration_date, long long cvv2);

static std::mutex mock_api_mutex;

extern bool effettua_pagamento_init(UDF_INIT* initid, const UDF_ARGS* args, char* message) {
	if (args->arg_count != 6
		|| args->arg_type[0] != INT_RESULT
		|| args->arg_type[1] != DECIMAL_RESULT
		|| args->arg_type[2] != STRING_RESULT
		|| args->arg_type[3] != INT_RESULT
		|| args->arg_type[4] != STRING_RESULT
		|| args->arg_type[5] != INT_RESULT) {
		strcpy(message, "effettua_pagamento() richiede 1 int, 1 decimale, 1 stringa, 1 int, 1 data, 1 int.");
		return true;
	}
	initid->const_item = false;
	initid->maybe_null = true;
	initid->max_length = 256;
	initid->ptr = (char*)malloc(initid->max_length);
	if (!initid->ptr) {
		strcpy(message, "Memoria insufficiente per eseguire effettua_pagamento().");
		return true;
	}
	return false;
}

extern char* effettua_pagamento(UDF_INIT* initid, const UDF_ARGS* args, const char*, unsigned long* length, const char*, char* error) {
	// TODO: implement a transaction history map <booking_id, transaction_id> in case of statement rollback.
	// The payment service API should have a built in timeout function, all this stuff should be trivial.
	using namespace std::chrono_literals;
	[[maybe_unused]] long long booking_id = *((long long*)args->args[0]);
	auto timeout = 10min;
	std::promise<std::string> transaction_id_promise;
	auto transaction_id_future = transaction_id_promise.get_future();
	auto backgroundThread = std::move(std::jthread{[&transaction_id_promise, args] {
		std::string price(args->args[1], args->lengths[1]);
		std::string name_on_card(args->args[1], args->lengths[1]);
		long long card_number = *((long long*)args->args[3]);
		std::string expiration_date(args->args[4], args->lengths[4]);
		long long cvv2 = *((long long*)args->args[5]);
		std::this_thread::sleep_for(50ms);	// RTT
		std::string transaction_id = mock_api_pay(price, name_on_card, card_number, expiration_date, cvv2);
		transaction_id_promise.set_value(transaction_id);
	}});
	if (auto status = transaction_id_future.wait_for(timeout); status == std::future_status::timeout) {
		*error = 1;
	}
	else if (status == std::future_status::ready) {
		std::string transaction_id = transaction_id_future.get();
		*length = (unsigned long)transaction_id.length();
		memcpy(initid->ptr, transaction_id.c_str(), *length);
	}
	return initid->ptr;
}

extern void effettua_pagamento_deinit(UDF_INIT* initid) {
	free(initid->ptr);
}

extern bool effettua_rimborso_init(UDF_INIT* initid, const UDF_ARGS* args, char* message) {
	if (args->arg_count != 1 || args->arg_type[0] != STRING_RESULT) {
		strcpy(message, "effettua_rimborso() richiede 1 parametro stringa.");
		return true;
	}
	initid->const_item = true;
	initid->maybe_null = false;
	return false;
}

extern long long effettua_rimborso(const UDF_INIT*, const UDF_ARGS*, const char*, const char*) {
	return 0;
}

extern void effettua_rimborso_deinit(const UDF_INIT*) {
	// this empty function is necessary to MySQL if the --allow-suspicious-udfs option is not set
}

static std::string mock_api_pay(
	[[maybe_unused]] std::string_view price,
	[[maybe_unused]] std::string_view name_on_card,
	[[maybe_unused]] long long card_number,
	[[maybe_unused]] std::string_view expiration_date,
	[[maybe_unused]] long long cvv2) {
		std::scoped_lock<std::mutex> lock(mock_api_mutex);
		auto now = std::chrono::system_clock::now();
	auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
	return std::to_string(timestamp);
}
