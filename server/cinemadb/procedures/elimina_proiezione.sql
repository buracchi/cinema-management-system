CREATE PROCEDURE `elimina_proiezione`(
    IN _cinema INT,
    IN _sala INT,
    IN _data DATE,
    IN _ora TIME)
BEGIN
    DECLARE _proiezione_inesistente CONDITION FOR SQLSTATE '45017';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45017);
    DECLARE _affected_rows INT;
    DELETE
    FROM `Proiezioni`
    WHERE `cinema` = _cinema
      AND `sala` = _sala
      AND `data` = _data
      AND `ora` = _ora;
    IF (_affected_rows = 0) THEN
        SIGNAL _proiezione_inesistente SET MESSAGE_TEXT = _err_msg;
    END IF;
END
