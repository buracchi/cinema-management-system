CREATE PROCEDURE `finalizza_prenotazione`(
    IN _codice VARCHAR(6),
    IN _intestatario VARCHAR(127),
    IN _numero_carta NUMERIC(16, 0),
    IN _scadenza DATE,
    IN _CVV2 NUMERIC(3))
BEGIN
    DECLARE _tid VARCHAR(255);
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            GET DIAGNOSTICS CONDITION 1 @var_errno = MYSQL_ERRNO;
            IF (@var_errno != 45022) THEN
                SELECT (EFFETTUA_RIMBORSO(_tid));
                ROLLBACK;
            END IF;
            RESIGNAL;
        END;
    SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
    START TRANSACTION;
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
        SET @err_msg = MESSAGGIO_ERRORE(45022);
        SIGNAL SQLSTATE '45022'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
    UPDATE `Prenotazioni`
    SET `stato`       = 'Confermata',
        `transazione` = _tid
    WHERE `codice` = _codice;
    COMMIT;
END
