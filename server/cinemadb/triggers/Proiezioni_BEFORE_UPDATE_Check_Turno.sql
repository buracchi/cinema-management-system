CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_UPDATE_Check_Turno`
    BEFORE UPDATE
    ON `Proiezioni`
    FOR EACH ROW
BEGIN
    DECLARE _periodo_scoperto CONDITION FOR SQLSTATE '45005';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45005);
    DECLARE _durata TIME;
    DECLARE esiste_turno BOOL;
    SET _durata = (SELECT `durata` FROM `Film` WHERE NEW.`film` = `id`);
    SET esiste_turno = (SELECT COUNT(*)
                        FROM `Turni`
                        WHERE `Turni`.`cinema` = NEW.`cinema`
                          AND `Turni`.`giorno` = GIORNO_DELLA_SETTIMANA(NEW.`data`)
                          AND `Turni`.`dipendente` = NEW.`proiezionista`
                          AND `inizio` <= NEW.`ora`
                          AND SEC_TO_TIME(TIME_TO_SEC(NEW.`ora`) + TIME_TO_SEC(_durata))
                            <= SEC_TO_TIME(TIME_TO_SEC(`inizio`) + TIME_TO_SEC(`durata`)));
    IF (NEW.`proiezionista` IS NOT NULL AND esiste_turno = FALSE) THEN
        SIGNAL _periodo_scoperto SET MESSAGE_TEXT = _err_msg;
    END IF;
END
