CREATE PROCEDURE `mostra_posti_disponibili`(
    IN _cinema_id INT,
    IN _sala_id INT,
    IN _data DATE,
    IN _ora TIME)
BEGIN
    IF (_cinema_id NOT IN (SELECT `id` FROM `Cinema`)) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45014);
        SIGNAL SQLSTATE '45014'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
    IF ((_cinema_id, _sala_id) NOT IN (SELECT `cinema`, `numero` FROM `Sale`)) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45018);
        SIGNAL SQLSTATE '45018'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
    IF ((_cinema_id, _sala_id, _data, _ora) NOT IN (SELECT `cinema`, `sala`, `data`, `ora` FROM `Proiezioni`)) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45017);
        SIGNAL SQLSTATE '45017'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
    SELECT `fila`, `numero`
    FROM `Posti`
    WHERE `Posti`.`cinema` = _cinema_id
      AND `Posti`.`sala` = _sala_id
      AND (`fila`, `numero`) NOT IN (SELECT `fila`, `numero`
                                     FROM `Prenotazioni`
                                     WHERE `Prenotazioni`.`cinema` = _cinema_id
                                       AND `Prenotazioni`.`sala` = _sala_id
                                       AND `Prenotazioni`.`data` = _data
                                       AND `Prenotazioni`.`ora` = _ora
                                       AND `Prenotazioni`.`stato` != 'Annullata');
END
