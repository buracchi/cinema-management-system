# EFFETTUA_PAGAMENTO(IN _prezzo DECIMAL(15, 2),
#	                 IN _intestatario VARCHAR(127),
#	                 IN _numero_carta NUMERIC(16,0),
#	                 IN _scadenza DATE,
#	                 IN _CVV2 NUMERIC(3))
#
# VALORE DI RITORNO: id univoco della transazione
#
DROP FUNCTION IF EXISTS EFFETTUA_PAGAMENTO;
CREATE FUNCTION EFFETTUA_PAGAMENTO RETURNS STRING SONAME "payment-service.dll";

# EFFETTUA_RIMBORSO(IN _codice_transazione VARCHAR(255))
#
# VALORE DI RITORNO: 0 in caso di successo
#
DROP FUNCTION IF EXISTS EFFETTUA_RIMBORSO;
CREATE FUNCTION EFFETTUA_RIMBORSO RETURNS INTEGER SONAME "payment-service.dll";
