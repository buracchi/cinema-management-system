CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_INSERT_Check_Sala_Libera`
BEFORE INSERT ON `Proiezioni`
FOR EACH ROW
BEGIN
	DECLARE inizio_proiezione TIMESTAMP;
	DECLARE fine_proiezione TIMESTAMP;
	DECLARE fine_proiezione_prec TIMESTAMP;
	DECLARE inizio_proiezione_succ TIMESTAMP;
    SET inizio_proiezione = TIMESTAMP(NEW.`data`, NEW.`ora`);
    SET fine_proiezione = TIMESTAMP(NEW.`data`, NEW.`ora` + (SELECT `durata` FROM `Film` WHERE `id` = NEW.`film`));
	SET inizio_proiezione_succ = (SELECT TIMESTAMP(`data`, `ora`)
										FROM `Proiezioni`
										WHERE `cinema` = NEW.`cinema` AND `sala` = NEW.`sala`
											AND (`data` > NEW.`data` OR (`data` = NEW.`data` AND `ora` > NEW.`ora`))
										ORDER BY `data` ASC, `ora` ASC
										LIMIT 1);
	SET fine_proiezione_prec = (SELECT TIMESTAMP(`data`, `ora` + `durata`)
										FROM `Proiezioni` JOIN `Film` on `id` = `film`
										WHERE `cinema` = NEW.`cinema` AND `sala` = NEW.`sala`
											AND (`data` < NEW.`data` OR (`data` = NEW.`data` AND `ora` <= NEW.`ora`))
										ORDER BY `data` DESC, `ora` DESC
										LIMIT 1);
    IF ((fine_proiezione_prec IS NOT NULL AND fine_proiezione_prec > inizio_proiezione)
		OR (inizio_proiezione_succ IS NOT NULL AND inizio_proiezione_succ < fine_proiezione)) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45002);
		SIGNAL SQLSTATE '45002'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END
