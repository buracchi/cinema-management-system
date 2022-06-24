CREATE PROCEDURE `annulla_prenotazione`(IN _codice CHAR(6))
BEGIN
    DECLARE _affected_rows INT;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;
    SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
    START TRANSACTION;
    UPDATE `Prenotazioni`
    SET `stato`='Annullata'
    WHERE `codice` = _codice;
    GET DIAGNOSTICS _affected_rows = ROW_COUNT;
    IF (_affected_rows = 0) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45013);
        SIGNAL SQLSTATE '45013'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
    IF (EFFETTUA_RIMBORSO((SELECT `transazione`
                           FROM `Prenotazioni`
                           WHERE `codice` = _codice)) != 0) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45022);
        SIGNAL SQLSTATE '45022'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
    COMMIT;
END
