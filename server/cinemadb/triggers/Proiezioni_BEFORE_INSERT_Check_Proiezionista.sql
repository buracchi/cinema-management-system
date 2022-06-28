CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_INSERT_Check_Proiezionista`
    BEFORE INSERT
    ON `Proiezioni`
    FOR EACH ROW
BEGIN
    DECLARE _dipendente_non_proiezionista CONDITION FOR SQLSTATE '45004';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45004);
    DECLARE _ruolo VARCHAR(15);
    SET _ruolo = (SELECT `ruolo` FROM `Dipendenti` WHERE `matricola` = NEW.`proiezionista`);
    IF (NEW.`proiezionista` IS NOT NULL AND _ruolo != 'Proiezionista') THEN
        SIGNAL _dipendente_non_proiezionista SET MESSAGE_TEXT = _err_msg;
    END IF;
END
