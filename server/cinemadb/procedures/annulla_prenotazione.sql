CREATE PROCEDURE `annulla_prenotazione`(IN _codice CHAR(6))
BEGIN
    DECLARE _codice_invalido CONDITION FOR SQLSTATE '45013';
    DECLARE _codice_invalido_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45013);
    DECLARE _rimborso_fallito CONDITION FOR SQLSTATE '45022';
    DECLARE _rimborso_fallito_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45022);
    DECLARE _affected_rows INT;
    DECLARE _tid VARCHAR(255);
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;
    -- Prenotazioni_BEFORE_UPDATE_Check_Duplicati necessita di impedire
    -- inserimenti fantasma, utilizzando il livello di serilizzabilita'
    -- SERIALIZABLE viene mantenuto un lock gap S sull'indice
    -- fk_Prenotazioni_Posti1_idx.
    SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
    START TRANSACTION;
    UPDATE `Prenotazioni`
    SET `stato`='Annullata'
    WHERE `codice` = _codice;
    GET DIAGNOSTICS _affected_rows = ROW_COUNT;
    IF (_affected_rows = 0) THEN
        SIGNAL _codice_invalido SET MESSAGE_TEXT = _codice_invalido_msg;
    END IF;
    SET _tid = (SELECT `transazione` FROM `Prenotazioni` WHERE `codice` = _codice);
    IF (EFFETTUA_RIMBORSO(_tid) != 0) THEN
        SIGNAL _rimborso_fallito SET MESSAGE_TEXT = _rimborso_fallito_msg;
    END IF;
    COMMIT;
END
