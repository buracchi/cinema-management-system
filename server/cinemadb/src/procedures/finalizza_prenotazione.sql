CREATE PROCEDURE `finalizza_prenotazione`(
    IN _codice VARCHAR(6),
    IN _intestatario VARCHAR(127),
    IN _numero_carta NUMERIC(16, 0),
    IN _scadenza DATE,
    IN _CVV2 NUMERIC(3))
BEGIN
    DECLARE _tid VARCHAR(255) DEFAULT NULL;
    DECLARE _codice_invalido CONDITION FOR SQLSTATE '45013';
    DECLARE _codice_invalido_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45013);
    DECLARE _pagamento_fallito CONDITION FOR SQLSTATE '45022';
    DECLARE _pagamento_fallito_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45022);
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            IF _tid IS NOT NULL THEN
                DO EFFETTUA_RIMBORSO(_tid);
            END IF;
            ROLLBACK;
            RESIGNAL;
        END;
    -- Prenotazioni_BEFORE_UPDATE_Check_Duplicati necessita di impedire
    -- inserimenti fantasma, utilizzando il livello di serilizzabilita'
    -- SERIALIZABLE viene mantenuto un lock gap S sull'indice
    -- fk_Prenotazioni_Posti1_idx.
    SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
    START TRANSACTION;
    IF (_codice NOT IN (SELECT `codice` FROM `Prenotazioni`)) THEN
        SIGNAL _codice_invalido SET MESSAGE_TEXT = _codice_invalido_msg;
    END IF;
    SET _tid = EFFETTUA_PAGAMENTO(
            (SELECT `prezzo`
             FROM `Prenotazioni`
                      NATURAL JOIN `Proiezioni`
             WHERE `codice` = _codice),
            _intestatario,
            _numero_carta,
            _scadenza,
            _CVV2);
    IF NOT _tid THEN
        SIGNAL _pagamento_fallito SET MESSAGE_TEXT = _pagamento_fallito_msg;
    END IF;
    UPDATE `Prenotazioni`
    SET `stato`       = 'Confermata',
        `transazione` = _tid
    WHERE `codice` = _codice;
    COMMIT;
END
