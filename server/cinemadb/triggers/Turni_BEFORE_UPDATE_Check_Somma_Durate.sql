CREATE TRIGGER `cinemadb`.`Turni_BEFORE_UPDATE_Check_Somma_Durate`
    BEFORE UPDATE
    ON `Turni`
    FOR EACH ROW
BEGIN
    DECLARE _turno_invalido CONDITION FOR SQLSTATE '45008';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45008);
    DECLARE OreTotali TIME;
    SET OreTotali = (SELECT SEC_TO_TIME(SUM(TIME_TO_SEC(`Turni`.`durata`)))
                     FROM `Turni`
                     WHERE `Turni`.`dipendente` = NEW.`dipendente`
                       AND `Turni`.`giorno` = NEW.`giorno`);
    IF (OreTotali + NEW.`durata` > TIME('08:00:00')) THEN
        SIGNAL _turno_invalido SET MESSAGE_TEXT = _err_msg;
    END IF;
END
