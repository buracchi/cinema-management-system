CREATE PROCEDURE `finalizza_prenotazione`(
    IN _codice VARCHAR(6),
    IN _intestatario VARCHAR(127),
    IN _numero_carta NUMERIC(16, 0),
    IN _scadenza DATE,
    IN _CVV2 NUMERIC(3))
BEGIN
    DECLARE var_tid VARCHAR(256);
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;
    SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
    START TRANSACTION;
    SET var_tid = EFFETTUA_PAGAMENTO(
            (SELECT `prezzo`
             FROM `Prenotazioni`
                      NATURAL JOIN `Proiezioni`
             WHERE `codice` = _codice),
            _intestatario,
            _numero_carta,
            _scadenza,
            _CVV2);
    IF NOT var_tid THEN
        SET @err_msg = MESSAGGIO_ERRORE(45022);
        SIGNAL SQLSTATE '45022'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
    UPDATE `Prenotazioni`
    SET `stato`       = 'Confermata',
        `transazione` = var_tid
    WHERE `codice` = _codice;
    COMMIT;
END
