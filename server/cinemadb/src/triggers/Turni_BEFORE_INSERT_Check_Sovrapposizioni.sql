CREATE TRIGGER `cinemadb`.`Turni_BEFORE_INSERT_Check_Sovrapposizioni`
    BEFORE INSERT
    ON `Turni`
    FOR EACH ROW
BEGIN
    DECLARE _dipendente_occupato CONDITION FOR SQLSTATE '45009';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45009);
    IF (EXISTS(SELECT *
               FROM `Turni`
               WHERE `Turni`.`dipendente` = NEW.`dipendente`
                 AND `Turni`.`giorno` = NEW.`giorno`
                 AND `Turni`.`inizio` <
                     (SEC_TO_TIME(TIME_TO_SEC(NEW.`inizio`)
                         + TIME_TO_SEC(NEW.`durata`)))
                 AND (SEC_TO_TIME(TIME_TO_SEC(`Turni`.`inizio`)
                   + TIME_TO_SEC(`Turni`.`durata`))) > NEW.`inizio`)) THEN
        SIGNAL _dipendente_occupato SET MESSAGE_TEXT = _err_msg;
    END IF;
END
