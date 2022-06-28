CREATE TRIGGER `cinemadb`.`Prenotazioni_BEFORE_UPDATE_Check_Stato`
    BEFORE UPDATE
    ON `Prenotazioni`
    FOR EACH ROW
BEGIN
    DECLARE _modifica_p_annulata_scaduta_invalida CONDITION FOR SQLSTATE '45011';
    DECLARE _modifica_p_annulata_scaduta_invalida_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45011);
    DECLARE _conferma_p_non_in_attesa CONDITION FOR SQLSTATE '45020';
    DECLARE _conferma_p_non_in_attesa_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45020);
    DECLARE _modifica_p_attesa_invalida CONDITION FOR SQLSTATE '45021';
    DECLARE _modifica_p_attesa_invalida_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45021);
    IF (OLD.`stato` = 'Attesa' && NEW.`stato` != 'Confermata') THEN
        SIGNAL _modifica_p_attesa_invalida SET MESSAGE_TEXT = _modifica_p_attesa_invalida_msg;
    ELSEIF (OLD.`stato` != 'Attesa' && NEW.`stato` = 'Confermata') THEN
        SIGNAL _conferma_p_non_in_attesa SET MESSAGE_TEXT = _conferma_p_non_in_attesa_msg;
    ELSEIF (OLD.`stato` != 'Attesa' && OLD.`stato` != 'Confermata') THEN
        SIGNAL _modifica_p_annulata_scaduta_invalida SET MESSAGE_TEXT = _modifica_p_annulata_scaduta_invalida_msg;
    END IF;
END
