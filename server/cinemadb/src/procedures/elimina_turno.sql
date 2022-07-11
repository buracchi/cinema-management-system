CREATE PROCEDURE `elimina_turno`(
    IN _dipendente INT,
    IN _giorno VARCHAR(15),
    IN _inizio TIME)
BEGIN
    DECLARE _turno_invalido CONDITION FOR SQLSTATE '45019';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45019);
    DECLARE _affected_rows INT;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;
    -- Turni_AFTER_DELETE_Reset_Proiezionista_Proiezione necessita di
    -- impedire inserimenti fantasma.
    SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
    START TRANSACTION;
    DELETE
    FROM `Turni`
    WHERE `dipendente` = _dipendente
      AND `giorno` = _giorno
      AND `inizio` = _inizio;
    GET DIAGNOSTICS _affected_rows = ROW_COUNT;
    IF (_affected_rows = 0) THEN
        SIGNAL _turno_invalido SET MESSAGE_TEXT = _err_msg;
    END IF;
    COMMIT;
END
