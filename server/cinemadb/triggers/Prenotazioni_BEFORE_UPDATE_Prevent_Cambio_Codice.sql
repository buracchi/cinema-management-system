CREATE TRIGGER `cinemadb`.`Prenotazioni_BEFORE_UPDATE_Prevent_Cambio_Codice`
    BEFORE UPDATE
    ON `Prenotazioni`
    FOR EACH ROW
BEGIN
    IF (OLD.`codice` != NEW.`codice`) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45023);
        SIGNAL SQLSTATE '45023'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
END
