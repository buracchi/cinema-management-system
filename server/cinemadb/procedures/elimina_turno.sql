CREATE PROCEDURE `elimina_turno`(
    IN _dipendente INT,
    IN _giorno VARCHAR(15),
    IN _inizio TIME)
BEGIN
    DECLARE _turno_inesistente CONDITION FOR SQLSTATE '45019';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45019);
    IF ((_dipendente, _giorno, _inizio) NOT IN (SELECT `dipendente`, `giorno`, `inizio`
                                                FROM `Turni`)) THEN
        SIGNAL _turno_inesistente SET MESSAGE_TEXT = _err_msg;
    END IF;
    DELETE
    FROM `Turni`
    WHERE `dipendente` = _dipendente
      AND `giorno` = _giorno
      AND `inizio` = _inizio;
END
