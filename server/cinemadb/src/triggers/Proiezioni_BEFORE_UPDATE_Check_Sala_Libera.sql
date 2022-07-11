CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_UPDATE_Check_Sala_Libera`
    BEFORE UPDATE
    ON `Proiezioni`
    FOR EACH ROW
BEGIN
    DECLARE _sala_impegnata CONDITION FOR SQLSTATE '45002';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45002);
    DECLARE _inizio_proiezione TIMESTAMP;
    DECLARE _fine_proiezione TIMESTAMP;
    DECLARE _fine_proiezione_prec TIMESTAMP;
    DECLARE _inizio_proiezione_succ TIMESTAMP;
    SET _inizio_proiezione = TIMESTAMP(NEW.`data`, NEW.`ora`);
    SET _fine_proiezione = TIMESTAMP(NEW.`data`, NEW.`ora` + (SELECT `durata` FROM `Film` WHERE `id` = NEW.`film`));
    SET _inizio_proiezione_succ = (SELECT TIMESTAMP(`data`, `ora`)
                                   FROM `Proiezioni`
                                   WHERE `cinema` = NEW.`cinema`
                                     AND `sala` = NEW.`sala`
                                     AND (`data` > NEW.`data` OR (`data` = NEW.`data` AND `ora` > NEW.`ora`))
                                     AND (`cinema`, `sala`, `data`, `ora`) NOT IN
                                         (SELECT OLD.`cinema`, OLD.`sala`, OLD.`data`, OLD.`ora`)
                                   ORDER BY `data`, `ora`
                                   LIMIT 1);
    SET _fine_proiezione_prec = (SELECT TIMESTAMP(`data`, `ora` + `durata`)
                                 FROM `Proiezioni`
                                          JOIN `Film` on `id` = `film`
                                 WHERE `cinema` = NEW.`cinema`
                                   AND `sala` = NEW.`sala`
                                   AND (`data` < NEW.`data` OR (`data` = NEW.`data` AND `ora` <= NEW.`ora`))
                                   AND (`cinema`, `sala`, `data`, `ora`) NOT IN
                                       (SELECT OLD.`cinema`, OLD.`sala`, OLD.`data`, OLD.`ora`)
                                 ORDER BY `data` DESC, `ora` DESC
                                 LIMIT 1);
    IF ((_fine_proiezione_prec IS NOT NULL AND _fine_proiezione_prec > _inizio_proiezione)
        OR (_inizio_proiezione_succ IS NOT NULL AND _inizio_proiezione_succ < _fine_proiezione)) THEN
        SIGNAL _sala_impegnata SET MESSAGE_TEXT = _err_msg;
    END IF;
END
