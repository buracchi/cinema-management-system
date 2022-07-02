CREATE PROCEDURE `elimina_sala`(
    IN _cinema INT,
    IN _numero INT)
BEGIN
    DECLARE _sala_invalida CONDITION FOR SQLSTATE '45018';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45018);
    DECLARE _affected_rows INT;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;
    SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
    START TRANSACTION;
    DELETE
    FROM `Sale`
    WHERE `cinema` = _cinema
      AND `numero` = _numero;
    GET DIAGNOSTICS _affected_rows = ROW_COUNT;
    IF (_affected_rows = 0) THEN
        SIGNAL _sala_invalida SET MESSAGE_TEXT = _err_msg;
    END IF;
    COMMIT;
END
