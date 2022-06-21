CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_INSERT_Check_Proiezionista`
    BEFORE INSERT
    ON `Proiezioni`
    FOR EACH ROW
BEGIN
    DECLARE _ruolo VARCHAR(15);
    SET _ruolo = (SELECT `ruolo` FROM `Dipendenti` WHERE `matricola` = NEW.`proiezionista`);
    IF (NEW.`proiezionista` IS NOT NULL AND _ruolo != 'Proiezionista') THEN
        SET @err_msg = MESSAGGIO_ERRORE(45004);
        SIGNAL SQLSTATE '45004'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
END
