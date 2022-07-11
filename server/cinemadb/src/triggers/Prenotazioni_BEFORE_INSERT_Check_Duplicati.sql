CREATE TRIGGER `cinemadb`.`Prenotazioni_BEFORE_INSERT_Check_Duplicati`
    BEFORE INSERT
    ON `Prenotazioni`
    FOR EACH ROW
BEGIN
    DECLARE _posto_occupato CONDITION FOR SQLSTATE '45015';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45015);
    DECLARE _esiste_duplicato BOOL;
    SET _esiste_duplicato = (SELECT COUNT(*)
                             FROM `Prenotazioni`
                             WHERE `cinema` = NEW.`cinema`
                               AND `sala` = NEW.`sala`
                               AND `fila` = NEW.`fila`
                               AND `numero` = NEW.`numero`
                               AND `data` = NEW.`data`
                               AND `ora` = NEW.`ora`
                               AND `stato` != 'Annullata');
    IF (_esiste_duplicato) THEN
        SIGNAL _posto_occupato SET MESSAGE_TEXT = _err_msg;
    END IF;
END
