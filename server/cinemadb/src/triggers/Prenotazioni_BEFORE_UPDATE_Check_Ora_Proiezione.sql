CREATE TRIGGER `cinemadb`.`Prenotazioni_BEFORE_UPDATE_Check_Ora_Proiezione`
    BEFORE UPDATE
    ON `Prenotazioni`
    FOR EACH ROW
BEGIN
    DECLARE _impossibile_annullare CONDITION FOR SQLSTATE '45012';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45012);
    DECLARE _inizio_proiezione TIMESTAMP;
    SET _inizio_proiezione = (SELECT TIMESTAMP(`data`, `ora`)
                              FROM `Prenotazioni`
                              WHERE `codice` = NEW.`codice`);
    IF (NEW.`stato` = 'Annullata'
        AND NOW() > DATE_SUB(_inizio_proiezione, INTERVAL 30 MINUTE)) THEN
        SIGNAL _impossibile_annullare SET MESSAGE_TEXT = _err_msg;
    END IF;
END
