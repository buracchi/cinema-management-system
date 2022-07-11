CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_UPDATE_Check_Proiezionista_Occupato`
    BEFORE UPDATE
    ON `Proiezioni`
    FOR EACH ROW
BEGIN
    DECLARE _proiezionista_occupato CONDITION FOR SQLSTATE '45006';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45006);
    DECLARE _fine TIME;
    DECLARE _impegnato BOOL;
    SET _fine = (SELECT SEC_TO_TIME(TIME_TO_SEC(NEW.`ora`) + TIME_TO_SEC(`durata`))
                 FROM `Film`
                 WHERE `id` = NEW.`film`);
    SET _impegnato = (SELECT COUNT(*)
                      FROM `Proiezioni`
                               JOIN `Film` ON `id` = `film`
                      WHERE `cinema` = NEW.`cinema`
                        AND `data` = NEW.`data`
                        AND `proiezionista` = NEW.`proiezionista`
                        AND `ora` <= _fine
                        AND SEC_TO_TIME(TIME_TO_SEC(`ora`)
                          + TIME_TO_SEC(`durata`)) >= NEW.`ora`);
    IF (NEW.`proiezionista` IS NOT NULL AND _impegnato = TRUE) THEN
        SIGNAL _proiezionista_occupato SET MESSAGE_TEXT = _err_msg;
    END IF;
END
