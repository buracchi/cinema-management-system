START TRANSACTION;
USE `cinemadb`;
INSERT INTO `Stati_Prenotazione` (`nome`)
VALUES ('Confermata');
INSERT INTO `Stati_Prenotazione` (`nome`)
VALUES ('Validata');
INSERT INTO `Stati_Prenotazione` (`nome`)
VALUES ('Scaduta');
INSERT INTO `Stati_Prenotazione` (`nome`)
VALUES ('Annullata');
INSERT INTO `Stati_Prenotazione` (`nome`)
VALUES ('Attesa');
COMMIT;
