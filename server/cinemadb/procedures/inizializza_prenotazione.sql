CREATE PROCEDURE `inizializza_prenotazione`(
    IN _cinema_id INT,
    IN _sala_id INT,
    IN _data DATE,
    IN _ora TIME,
    IN _fila CHAR(1),
    IN _numero INT)
BEGIN
    INSERT INTO `Prenotazioni` (`codice`, `stato`, `cinema`, `sala`, `data`, `ora`, `fila`, `numero`, `timestamp`)
    VALUES (LAST_INSERT_ID(GENERA_CODICE_PRENOTAZIONE()), 'Attesa', _cinema_id, _sala_id, _data, _ora, _fila, _numero,
            NOW());
    SELECT HEX(LAST_INSERT_ID());
END
