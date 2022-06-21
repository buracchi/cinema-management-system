CREATE PROCEDURE `mostra_proiezionisti_disponibili`(
    IN _cinema INT,
    IN _sala INT,
    IN _data DATE,
    IN _ora TIME)
BEGIN
    IF (_cinema NOT IN (SELECT `id` FROM `Cinema`)) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45014);
        SIGNAL SQLSTATE '45014'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
    IF ((_cinema, _sala) NOT IN (SELECT `cinema`, `numero` FROM `Sale`)) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45018);
        SIGNAL SQLSTATE '45018'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
    SELECT `matricola`, `Dipendenti`.`nome`, `cognome`
    FROM `Dipendenti`
             JOIN `Turni` ON `matricola` = `dipendente`
             JOIN `Proiezioni` AS P ON P.`cinema` = `Turni`.`cinema`
        AND GIORNO_DELLA_SETTIMANA(P.`data`) = `Turni`.`giorno`
             JOIN `Film` AS F ON `id` = P.`film`
    WHERE P.`cinema` = _cinema
      AND P.`sala` = _sala
      AND P.`data` = _data
      AND P.`ora` = _ora
      AND `ruolo` = 'Proiezionista'
      AND `inizio` <= _ora
      AND SEC_TO_TIME(TIME_TO_SEC(`Turni`.`inizio`) + TIME_TO_SEC(`Turni`.`durata`))
        >= SEC_TO_TIME(TIME_TO_SEC(P.`ora`) + TIME_TO_SEC(F.`durata`))
      AND NOT EXISTS(SELECT *
                     FROM `Proiezioni`
                              JOIN `Film` ON `id` = `film`
                     WHERE `cinema` = _cinema
                       AND `data` = _data
                       AND `ora` <= SEC_TO_TIME(TIME_TO_SEC(P.`ora`) + TIME_TO_SEC(F.`durata`))
                       AND SEC_TO_TIME(TIME_TO_SEC(`ora`) + TIME_TO_SEC(`durata`)) >= _ora
                       AND `proiezionista` = `matricola`);
END
