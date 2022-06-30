CREATE PROCEDURE `elimina_cinema`(IN _id INT)
BEGIN
    DECLARE _cinema_inesistente CONDITION FOR SQLSTATE '45014';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45014);
    DECLARE _affected_rows INT;
    DELETE
    FROM `Cinema`
    WHERE `id` = _id;
    GET DIAGNOSTICS _affected_rows = ROW_COUNT;
    IF (_affected_rows = 0) THEN
        SIGNAL _cinema_inesistente SET MESSAGE_TEXT = _err_msg;
    END IF;
END
