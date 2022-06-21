CREATE TRIGGER `cinemadb`.`Prenotazioni_BEFORE_INSERT_Check_Duplicati`
BEFORE INSERT ON `Prenotazioni`
FOR EACH ROW
BEGIN
	DECLARE esiste_duplicato BOOL;
    SET esiste_duplicato = (SELECT COUNT(*)
								FROM `Prenotazioni`
                                WHERE `cinema` = NEW.`cinema`
									AND `sala` = NEW.`sala`
									AND `fila` = NEW.`fila`
									AND `numero` = NEW.`numero`
									AND `data` = NEW.`data`
									AND `ora` = NEW.`ora`
                                    AND `stato` != 'Annullata');
    IF (esiste_duplicato) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45015);
		SIGNAL SQLSTATE '45015'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END
