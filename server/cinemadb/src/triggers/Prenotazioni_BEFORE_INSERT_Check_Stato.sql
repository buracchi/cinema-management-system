CREATE TRIGGER `cinemadb`.`Prenotazioni_BEFORE_INSERT_Check_Stato`
    BEFORE INSERT
    ON `Prenotazioni`
    FOR EACH ROW
BEGIN
    DECLARE _prenotazione_invalida CONDITION FOR SQLSTATE '45010';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45010);
    IF (NEW.`stato` != 'Attesa') THEN
        SIGNAL _prenotazione_invalida SET MESSAGE_TEXT = _err_msg;
    END IF;
END
