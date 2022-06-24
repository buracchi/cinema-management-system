CREATE PROCEDURE `inizializza_prenotazione`(
    IN _cinema_id INT,
    IN _sala_id INT,
    IN _data DATE,
    IN _ora TIME,
    IN _fila CHAR(1),
    IN _numero INT)
BEGIN
    DECLARE _codice CHAR(6);
    DECLARE _fine BOOL DEFAULT TRUE;
    DECLARE _codice_doppione CONDITION FOR 1062;
    retry:
    REPEAT
        BEGIN
            DECLARE CONTINUE HANDLER FOR _codice_doppione BEGIN
                SET _fine = FALSE;
            END;
            SET _fine = TRUE;
            SET _codice = GENERA_CODICE_PRENOTAZIONE();
            INSERT INTO `Prenotazioni` (`codice`, `stato`, `cinema`, `sala`, `data`, `ora`, `fila`, `numero`,
                                        `timestamp`)
            VALUES (_codice, 'Attesa', _cinema_id, _sala_id, _data, _ora, _fila, _numero, NOW());
        END;
    UNTIL _fine END REPEAT;
    SELECT _codice;
END
