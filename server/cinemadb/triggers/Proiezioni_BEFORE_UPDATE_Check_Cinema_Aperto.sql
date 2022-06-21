CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_UPDATE_Check_Cinema_Aperto`
BEFORE UPDATE ON `Proiezioni`
FOR EACH ROW
BEGIN
	DECLARE ora_apertura_cinema TIME;
    DECLARE ora_chiusura_cinema TIME;
    DECLARE durata TIME;
    SET ora_apertura_cinema = (SELECT `apertura` FROM `Cinema` WHERE `id` = NEW.`cinema`);
    SET ora_chiusura_cinema = (SELECT `chiusura` FROM `Cinema` WHERE `id` = NEW.`cinema`);
    SET durata = (SELECT `durata` FROM `Film` WHERE `id` = NEW.`film`);
    IF (NEW.`ora` < ora_apertura_cinema
		OR SEC_TO_TIME(TIME_TO_SEC(NEW.`ora`) + TIME_TO_SEC(durata)) > ora_chiusura_cinema) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45003);
		SIGNAL SQLSTATE '45003'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END
