CREATE PROCEDURE `elimina_turno`(
    IN _dipendente INT,
    IN _giorno VARCHAR(15),
    IN _inizio TIME)
BEGIN
    DECLARE _turno_inesistente CONDITION FOR SQLSTATE '45019';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45019);
    DECLARE _affected_rows INT;
    DELETE
    FROM `Turni`
    WHERE `dipendente` = _dipendente
      AND `giorno` = _giorno
      AND `inizio` = _inizio;
    IF (_affected_rows = 0) THEN
        SIGNAL _turno_inesistente SET MESSAGE_TEXT = _err_msg;
    END IF;
END
