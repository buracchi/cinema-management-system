CREATE PROCEDURE `elimina_turno` (
	IN _dipendente INT,
	IN _giorno VARCHAR(15),
	IN _inizio TIME)
BEGIN
	IF ((_dipendente, _giorno, _inizio) NOT IN (SELECT `dipendente`, `giorno`, `inizio` FROM `Turni`)) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45019);
		SIGNAL SQLSTATE '45019'
		SET MESSAGE_TEXT = @err_msg;
	END IF;
	DELETE FROM `Turni`
    WHERE `dipendente` = _dipendente
		AND `giorno` = _giorno
		AND `inizio` = _inizio;
END
