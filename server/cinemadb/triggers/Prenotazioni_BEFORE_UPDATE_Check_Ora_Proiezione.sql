CREATE TRIGGER `cinemadb`.`Prenotazioni_BEFORE_UPDATE_Check_Ora_Proiezione`
    BEFORE UPDATE
    ON `Prenotazioni`
    FOR EACH ROW
BEGIN
    DECLARE inizio_proiezione TIMESTAMP;
    SET inizio_proiezione = (SELECT TIMESTAMP(`data`, `ora`)
                             FROM `Prenotazioni`
                             WHERE `codice` = NEW.`codice`);
    IF (NEW.`stato` = 'Annullata'
        AND NOW() > DATE_SUB(inizio_proiezione, INTERVAL 30 MINUTE)) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45012);
        SIGNAL SQLSTATE '45012'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
END
