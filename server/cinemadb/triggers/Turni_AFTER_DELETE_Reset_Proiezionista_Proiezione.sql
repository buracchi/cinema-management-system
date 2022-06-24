CREATE TRIGGER `cinemadb`.`Turni_AFTER_DELETE_Reset_Proiezionista_Proiezione`
    AFTER DELETE
    ON `Turni`
    FOR EACH ROW
BEGIN
    WITH P AS (SELECT `cinema`, `sala`, `data`, `ora`
               FROM `Proiezioni`
                        JOIN `Film` ON `film` = `id`
               WHERE `proiezionista` = OLD.`dipendente`
                 AND `cinema` = OLD.`cinema`
                 AND GIORNO_DELLA_SETTIMANA(`data`) = OLD.`giorno`
                 AND `ora` >= OLD.`inizio`
                 AND SEC_TO_TIME(TIME_TO_SEC(`ora`) + TIME_TO_SEC(`Film`.`durata`))
                   <= SEC_TO_TIME(TIME_TO_SEC(OLD.`inizio`) + TIME_TO_SEC(OLD.`durata`)))
    UPDATE `Proiezioni`
    SET `proiezionista` = NULL
    WHERE `cinema` = P.`cinema`
      AND `sala` = P.`sala`
      AND `data` = P.`data`
      AND `ora` = P.`ora`;
END
