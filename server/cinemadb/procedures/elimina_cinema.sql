CREATE PROCEDURE `elimina_cinema`(IN _id INT)
BEGIN
    DECLARE _cinema_invalida CONDITION FOR SQLSTATE '45014';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45014);
    DECLARE _affected_rows INT;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;
    SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
    START TRANSACTION;
    DELETE
    FROM `Cinema`
    WHERE `id` = _id;
    GET DIAGNOSTICS _affected_rows = ROW_COUNT;
    IF (_affected_rows = 0) THEN
        SIGNAL _cinema_invalida SET MESSAGE_TEXT = _err_msg;
    END IF;
    COMMIT;
END
