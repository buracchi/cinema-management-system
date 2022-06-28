CREATE TRIGGER `cinemadb`.`Turni_BEFORE_INSERT_Check_Cinema_Aperto`
    BEFORE INSERT
    ON `Turni`
    FOR EACH ROW
BEGIN
    DECLARE _cinema_chiuso CONDITION FOR SQLSTATE '45003';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45003);
    DECLARE ora_apertura_cinema TIME;
    DECLARE ora_chiusura_cinema TIME;
    SET ora_apertura_cinema = (SELECT `apertura` FROM `Cinema` WHERE `id` = NEW.`cinema`);
    SET ora_chiusura_cinema = (SELECT `chiusura` FROM `Cinema` WHERE `id` = NEW.`cinema`);
    IF (NEW.`inizio` < ora_apertura_cinema
        OR SEC_TO_TIME(TIME_TO_SEC(NEW.`inizio`)
            + TIME_TO_SEC(NEW.`durata`)) > ora_chiusura_cinema) THEN
        SIGNAL _cinema_chiuso SET MESSAGE_TEXT = _err_msg;
    END IF;
END
