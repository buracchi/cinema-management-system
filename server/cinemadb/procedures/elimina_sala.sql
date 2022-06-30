CREATE PROCEDURE `elimina_sala`(
    IN _cinema INT,
    IN _numero INT)
BEGIN
    DECLARE _sala_inesistente CONDITION FOR SQLSTATE '45018';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45018);
    DECLARE _affected_rows INT;
    DELETE
    FROM `Sale`
    WHERE `cinema` = _cinema
      AND `numero` = _numero;
    IF (_affected_rows = 0) THEN
        SIGNAL _sala_inesistente SET MESSAGE_TEXT = _err_msg;
    END IF;
END
