CREATE PROCEDURE `elimina_proiezione` (
    IN _cinema INT,
	IN _sala INT,
	IN _data DATE,
	IN _ora TIME)
BEGIN
	IF ((_cinema, _sala, _data, _ora) NOT IN (SELECT `cinema`, `sala`, `data`, `ora` FROM `Proiezioni`)) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45017);
		SIGNAL SQLSTATE '45017'
		SET MESSAGE_TEXT = @err_msg;
	END IF;
	DELETE FROM `Proiezioni`
    WHERE `cinema` = _cinema AND `sala` = _sala
		AND `data` = _data AND `ora` = _ora;
END
