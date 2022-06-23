CREATE PROCEDURE `valida_prenotazione`(IN _codice CHAR(6))
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
    SET `stato` = 'Validata'
    WHERE `codice` = _codice;
    COMMIT;
END
