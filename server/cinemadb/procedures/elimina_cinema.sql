CREATE PROCEDURE `elimina_cinema` (IN _id INT)
BEGIN
	IF (_id NOT IN (SELECT `id` FROM `Cinema`)) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45014);
		SIGNAL SQLSTATE '45014'
		SET MESSAGE_TEXT = @err_msg;
	END IF;
	DELETE FROM `Cinema`
    WHERE `id` = _id;
END
