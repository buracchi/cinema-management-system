CREATE TRIGGER `cinemadb`.`Turni_BEFORE_INSERT_Check_Sovrapposizioni`
BEFORE INSERT ON `Turni`
FOR EACH ROW
BEGIN
    DECLARE clash BOOL;
    SET clash = EXISTS (SELECT *
                        FROM `Turni`
                        WHERE `Turni`.`dipendente` = NEW.`dipendente`
                            AND `Turni`.`giorno` = NEW.`giorno`
                            AND `Turni`.`inizio` <
                            (SEC_TO_TIME(TIME_TO_SEC(NEW.`inizio`)
                            + TIME_TO_SEC(NEW.`durata`)))
                            AND (SEC_TO_TIME(TIME_TO_SEC(`Turni`.`inizio`)
                            + TIME_TO_SEC(`Turni`.`durata`))) > NEW.`inizio`);
    IF (clash) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45009);
		SIGNAL SQLSTATE '45009'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END
