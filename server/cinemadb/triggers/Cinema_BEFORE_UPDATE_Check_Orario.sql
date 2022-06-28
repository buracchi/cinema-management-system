CREATE TRIGGER `cinemadb`.`Cinema_BEFORE_UPDATE_Check_Orario`
    BEFORE UPDATE
    ON `Cinema`
    FOR EACH ROW
BEGIN
    DECLARE _chiusura_precede_apertura CONDITION FOR SQLSTATE '45001';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45001);
    IF (NEW.`chiusura` < NEW.`apertura`) THEN
        SIGNAL _chiusura_precede_apertura SET MESSAGE_TEXT = _err_msg;
    END IF;
END
