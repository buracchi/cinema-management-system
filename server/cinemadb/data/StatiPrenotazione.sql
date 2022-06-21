START TRANSACTION;
USE `cinemadb`;
INSERT INTO `cinemadb`.`StatiPrenotazione` (`nome`)
VALUES ('Confermata');
INSERT INTO `cinemadb`.`StatiPrenotazione` (`nome`)
VALUES ('Validata');
INSERT INTO `cinemadb`.`StatiPrenotazione` (`nome`)
VALUES ('Scaduta');
INSERT INTO `cinemadb`.`StatiPrenotazione` (`nome`)
VALUES ('Annullata');
INSERT INTO `cinemadb`.`StatiPrenotazione` (`nome`)
VALUES ('Attesa');

COMMIT;
