START TRANSACTION;
USE `cinemadb`;
INSERT INTO `cinemadb`.`Stati_Prenotazione` (`nome`)
VALUES ('Confermata');
INSERT INTO `cinemadb`.`Stati_Prenotazione` (`nome`)
VALUES ('Validata');
INSERT INTO `cinemadb`.`Stati_Prenotazione` (`nome`)
VALUES ('Scaduta');
INSERT INTO `cinemadb`.`Stati_Prenotazione` (`nome`)
VALUES ('Annullata');
INSERT INTO `cinemadb`.`Stati_Prenotazione` (`nome`)
VALUES ('Attesa');
COMMIT;
