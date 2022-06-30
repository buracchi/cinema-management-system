CREATE PROCEDURE `elimina_dipendente`(IN _matricola INT)
BEGIN
    DECLARE _dipendente_inesistente CONDITION FOR SQLSTATE '45016';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45016);
    DECLARE _affected_rows INT;
    DELETE
    FROM `Dipendenti`
    WHERE `matricola` = _matricola;
    GET DIAGNOSTICS _affected_rows = ROW_COUNT;
    IF (_affected_rows = 0) THEN
        SIGNAL _dipendente_inesistente SET MESSAGE_TEXT = _err_msg;
    END IF;
END
