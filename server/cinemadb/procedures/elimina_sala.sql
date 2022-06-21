CREATE PROCEDURE `elimina_sala` (
	IN _cinema INT,
	IN _numero INT)
BEGIN
	IF ((_cinema, _numero) NOT IN (SELECT `cinema`, `numero` FROM `Sale`)) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45018);
		SIGNAL SQLSTATE '45018'
		SET MESSAGE_TEXT = @err_msg;
	END IF;
	DELETE FROM `Sale`
    WHERE `cinema` = _cinema AND `numero` = _numero;
END
