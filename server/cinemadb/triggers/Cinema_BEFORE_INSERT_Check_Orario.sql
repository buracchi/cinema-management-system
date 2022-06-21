CREATE TRIGGER `cinemadb`.`Cinema_BEFORE_INSERT_Check_Orario`
    BEFORE INSERT
    ON `Cinema`
    FOR EACH ROW
BEGIN
    IF (NEW.`chiusura` < NEW.`apertura`) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45001);
        SIGNAL SQLSTATE '45001'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
END
