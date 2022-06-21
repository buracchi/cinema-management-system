CREATE TRIGGER `cinemadb`.`Turni_BEFORE_INSERT_Check_Durata`
    BEFORE INSERT
    ON `Turni`
    FOR EACH ROW
BEGIN
    IF (NEW.`durata` > TIME('08:00:00')) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45007);
        SIGNAL SQLSTATE '45007'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
END
