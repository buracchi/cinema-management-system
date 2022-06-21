DROP FUNCTION IF EXISTS EFFETTUA_PAGAMENTO;
CREATE FUNCTION EFFETTUA_PAGAMENTO RETURNS STRING SONAME "payment-service.dll";
# SEGNATURA:
# EFFETTUA_PAGAMENTO(IN _codice_prenotazione INT,
#                    IN _prezzo DECIMAL(15, 2),
#	                 IN _intestatario VARCHAR(128),
#	                 IN _numero_carta NUMERIC(16,0),
#	                 IN _scadenza DATE,
#	                 IN _CVV2 NUMERIC(3))

DROP FUNCTION IF EXISTS EFFETTUA_RIMBORSO;
CREATE FUNCTION EFFETTUA_RIMBORSO RETURNS INTEGER SONAME "payment-service.dll";
# SEGNATURA:
# EFFETTUA_RIMBORSO(IN _codice_transazione VARCHAR(256))
