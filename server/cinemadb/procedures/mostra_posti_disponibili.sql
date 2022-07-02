CREATE PROCEDURE `mostra_posti_disponibili`(
    IN _cinema_id INT,
    IN _sala_id INT,
    IN _data DATE,
    IN _ora TIME)
BEGIN
    DECLARE _cinema_inesistente CONDITION FOR SQLSTATE '45014';
    DECLARE _cinema_inesistente_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45014);
    DECLARE _sala_inesistente CONDITION FOR SQLSTATE '45018';
    DECLARE _sala_inesistente_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45018);
    DECLARE _proiezione_inesistente CONDITION FOR SQLSTATE '45017';
    DECLARE _proiezione_inesistente_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45017);
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            RESIGNAL;
        END;
    SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
    START TRANSACTION;
    IF (_cinema_id NOT IN (SELECT `id` FROM `Cinema`)) THEN
        SIGNAL _cinema_inesistente SET MESSAGE_TEXT = _cinema_inesistente_msg;
    END IF;
    IF ((_cinema_id, _sala_id) NOT IN (SELECT `cinema`, `numero` FROM `Sale`)) THEN
        SIGNAL _sala_inesistente SET MESSAGE_TEXT = _sala_inesistente_msg;
    END IF;
    IF ((_cinema_id, _sala_id, _data, _ora) NOT IN (SELECT `cinema`, `sala`, `data`, `ora`
                                                    FROM `Proiezioni`)) THEN
        SIGNAL _proiezione_inesistente SET MESSAGE_TEXT = _proiezione_inesistente_msg;
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
    COMMIT;
END
