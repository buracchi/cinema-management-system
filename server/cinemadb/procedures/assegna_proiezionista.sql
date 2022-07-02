CREATE PROCEDURE `assegna_proiezionista`(
    IN _proiezionista INT,
    IN _cinema INT,
    IN _sala INT,
    IN _data DATE,
    IN _ora TIME)
BEGIN
    DECLARE _proiezione_invalida CONDITION FOR SQLSTATE '45017';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45017);
    DECLARE _affected_rows INT;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;
    SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
    START TRANSACTION;
    UPDATE `Proiezioni`
    SET `proiezionista` = _proiezionista
    WHERE `cinema` = _cinema
      AND `sala` = _sala
      AND `data` = _data
      AND `ora` = _ora;
    GET DIAGNOSTICS _affected_rows = ROW_COUNT;
    IF (_affected_rows = 0) THEN
        SIGNAL _proiezione_invalida SET MESSAGE_TEXT = _err_msg;
    END IF;
    COMMIT;
END
