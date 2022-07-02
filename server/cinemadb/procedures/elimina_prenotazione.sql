CREATE PROCEDURE `elimina_prenotazione`(IN _codice CHAR(6))
BEGIN
    DECLARE _codice_invalido CONDITION FOR SQLSTATE '45013';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45013);
    DECLARE _affected_rows INT;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;
    SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
    START TRANSACTION;
    DELETE
    FROM `Prenotazioni`
    WHERE `codice` = _codice
      AND `stato` = 'Attesa';
    GET DIAGNOSTICS _affected_rows = ROW_COUNT;
    IF (_affected_rows = 0) THEN
        SIGNAL _codice_invalido SET MESSAGE_TEXT = _err_msg;
    END IF;
    COMMIT;
END
