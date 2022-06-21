CREATE TRIGGER `cinemadb`.`Prenotazioni_BEFORE_UPDATE_Check_Duplicati`
    BEFORE UPDATE
    ON `Prenotazioni`
    FOR EACH ROW
BEGIN
    DECLARE esiste_duplicato BOOL;
    SET esiste_duplicato = (SELECT COUNT(*)
                            FROM `Prenotazioni`
                            WHERE (`cinema`, `sala`, `fila`, `numero`, `data`, `ora`) NOT IN
                                  (SELECT OLD.`cinema`, OLD.`sala`, OLD.`fila`, OLD.`numero`, OLD.`data`, OLD.`ora`)
                              AND `cinema` = NEW.`cinema`
                              AND `sala` = NEW.`sala`
                              AND `fila` = NEW.`fila`
                              AND `numero` = NEW.`numero`
                              AND `data` = NEW.`data`
                              AND `ora` = NEW.`ora`
                              AND `stato` != 'Annullata');
    IF (esiste_duplicato) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45015);
        SIGNAL SQLSTATE '45015'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
END
