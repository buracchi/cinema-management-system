CREATE TRIGGER `cinemadb`.`Turni_BEFORE_INSERT_Check_Durata`
    BEFORE INSERT
    ON `Turni`
    FOR EACH ROW
BEGIN
    DECLARE _turno_invalido CONDITION FOR SQLSTATE '45007';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45007);
    IF (NEW.`durata` > TIME('08:00:00')) THEN
        SIGNAL _turno_invalido SET MESSAGE_TEXT = _err_msg;
    END IF;
END
