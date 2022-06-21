CREATE TRIGGER `cinemadb`.`Turni_BEFORE_INSERT_Check_Somma_Durate`
    BEFORE INSERT
    ON `Turni`
    FOR EACH ROW
BEGIN
    DECLARE OreTotali TIME;
    SET OreTotali = (SELECT SEC_TO_TIME(SUM(TIME_TO_SEC(`Turni`.`durata`)))
                     FROM `Turni`
                     WHERE `Turni`.`dipendente` = NEW.`dipendente`
                       AND `Turni`.`giorno` = NEW.`giorno`);
    IF (OreTotali + NEW.`durata` > TIME('08:00:00')) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45008);
        SIGNAL SQLSTATE '45008'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
END
