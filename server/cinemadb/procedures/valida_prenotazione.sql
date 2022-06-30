CREATE PROCEDURE `valida_prenotazione`(IN _codice CHAR(6))
BEGIN
    DECLARE _codice_invalido CONDITION FOR SQLSTATE '45013';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45013);
    DECLARE _affected_rows INT;
    UPDATE `Prenotazioni`
    SET `stato` = 'Validata'
    WHERE `codice` = _codice;
    GET DIAGNOSTICS _affected_rows = ROW_COUNT;
    IF (_affected_rows = 0) THEN
        SIGNAL _codice_invalido SET MESSAGE_TEXT = _err_msg;
    END IF;
END
