CREATE PROCEDURE `annulla_prenotazione`(IN _codice CHAR(6))
BEGIN
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;
    SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
    START TRANSACTION;
    IF (_codice NOT IN (SELECT `codice` FROM `Prenotazioni`)) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45013);
        SIGNAL SQLSTATE '45013'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
    UPDATE `Prenotazioni`
    SET `stato`='Annullata'
    WHERE `codice` = _codice;
    IF (EFFETTUA_RIMBORSO((SELECT `transazione`
                           FROM `Prenotazioni`
                           WHERE `codice` = _codice)) != 0) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45022);
        SIGNAL SQLSTATE '45022'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
    COMMIT;
END
