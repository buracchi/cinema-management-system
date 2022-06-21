CREATE PROCEDURE `elimina_dipendente`(IN _matricola INT)
BEGIN
    IF (_matricola NOT IN (SELECT `matricola` FROM `Dipendenti`)) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45016);
        SIGNAL SQLSTATE '45016'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
    DELETE
    FROM `Dipendenti`
    WHERE `matricola` = _matricola;
END
