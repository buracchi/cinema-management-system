CREATE TRIGGER `cinemadb`.`Prenotazioni_BEFORE_UPDATE_Check_Stato`
BEFORE UPDATE ON `Prenotazioni`
FOR EACH ROW
BEGIN
	IF (OLD.`stato` = 'Attesa' AND NEW.`stato` != 'Confermata') THEN
		SET @err_msg = MESSAGGIO_ERRORE(45021);
		SIGNAL SQLSTATE '45021'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
	IF (OLD.`stato` != 'Attesa' AND NEW.`stato` = 'Confermata') THEN
		SET @err_msg = MESSAGGIO_ERRORE(45020);
		SIGNAL SQLSTATE '45020'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
	IF (OLD.`stato` != 'Attesa' AND OLD.`stato` != 'Confermata') THEN
		SET @err_msg = MESSAGGIO_ERRORE(45011);
		SIGNAL SQLSTATE '45011'
		SET MESSAGE_TEXT = @err_msg;
	END IF;
END
