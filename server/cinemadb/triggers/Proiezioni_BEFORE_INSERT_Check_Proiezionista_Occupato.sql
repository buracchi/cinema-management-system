CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_INSERT_Check_Proiezionista_Occupato`
BEFORE INSERT ON `Proiezioni`
FOR EACH ROW
BEGIN
	DECLARE _fine TIME;
	DECLARE impegnato BOOL;
    SET _fine = (SELECT SEC_TO_TIME(TIME_TO_SEC(NEW.`ora`) + TIME_TO_SEC(`durata`))
					FROM `Film`
                    WHERE `id` = NEW.`film`);
    SET impegnato = (SELECT COUNT(*)
						FROM `Proiezioni` JOIN `Film` ON `id` = `film`
						WHERE `cinema` = NEW.`cinema`
						AND `data` = NEW.`data`
						AND `proiezionista` = NEW.`proiezionista`
						AND `ora` <= _fine
						AND SEC_TO_TIME(TIME_TO_SEC(`ora`) + TIME_TO_SEC(`durata`))
							>= NEW.`ora`);
	IF (NEW.`proiezionista` IS NOT NULL AND impegnato = TRUE) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45006);
		SIGNAL SQLSTATE '45006'
		SET MESSAGE_TEXT = @err_msg;
	END IF;
END
