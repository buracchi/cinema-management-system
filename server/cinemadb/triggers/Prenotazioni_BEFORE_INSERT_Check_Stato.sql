CREATE TRIGGER `cinemadb`.`Prenotazioni_BEFORE_INSERT_Check_Stato`
BEFORE INSERT ON `Prenotazioni`
FOR EACH ROW
BEGIN
	IF (NEW.`stato` != 'Attesa') THEN
		SET @err_msg = MESSAGGIO_ERRORE(45010);
		SIGNAL SQLSTATE '45010'
		SET MESSAGE_TEXT = @err_msg;
	END IF;
END
