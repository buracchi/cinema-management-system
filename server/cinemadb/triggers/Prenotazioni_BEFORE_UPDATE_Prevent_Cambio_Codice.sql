CREATE TRIGGER `cinemadb`.`Prenotazioni_BEFORE_UPDATE_Prevent_Cambio_Codice`
    BEFORE UPDATE
    ON `Prenotazioni`
    FOR EACH ROW
BEGIN
    DECLARE _cambio_codice CONDITION FOR SQLSTATE '45023';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45023);
    IF (OLD.`codice` != NEW.`codice`) THEN
        SIGNAL _cambio_codice SET MESSAGE_TEXT = _err_msg;
    END IF;
END
