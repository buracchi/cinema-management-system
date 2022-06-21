CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_INSERT_Check_Turno`
BEFORE INSERT ON `Proiezioni`
FOR EACH ROW
BEGIN
    DECLARE _durata TIME;
	DECLARE esiste_turno BOOL;
    SET _durata = (SELECT `durata` FROM `Film` WHERE NEW.`film` = `id`);
	SET esiste_turno = (SELECT COUNT(*)
						FROM `Turni`
						WHERE `Turni`.`cinema` = NEW.`cinema`
							AND `Turni`.`giorno` = GIORNO_DELLA_SETTIMANA(NEW.`data`)
							AND `Turni`.`dipendente` = NEW.`proiezionista`
							AND `inizio` <= NEW.`ora`
							AND SEC_TO_TIME(TIME_TO_SEC(NEW.`ora`) + TIME_TO_SEC(_durata))
							<= SEC_TO_TIME(TIME_TO_SEC(`inizio`) + TIME_TO_SEC(`durata`)));
	IF (NEW.`proiezionista` IS NOT NULL AND esiste_turno = FALSE) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45005);
		SIGNAL SQLSTATE '45005'
		SET MESSAGE_TEXT = @err_msg;
	END IF;
END
