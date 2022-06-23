START TRANSACTION;
USE `cinemadb`;
INSERT INTO `cinemadb`.`Giorni` (`nome`, `numero`)
VALUES ("Lunedi'", 1);
INSERT INTO `cinemadb`.`Giorni` (`nome`, `numero`)
VALUES ("Martedi'", 2);
INSERT INTO `cinemadb`.`Giorni` (`nome`, `numero`)
VALUES ("Mercoledi'", 3);
INSERT INTO `cinemadb`.`Giorni` (`nome`, `numero`)
VALUES ("Giovedi'", 4);
INSERT INTO `cinemadb`.`Giorni` (`nome`, `numero`)
VALUES ("Venerdi'", 5);
INSERT INTO `cinemadb`.`Giorni` (`nome`, `numero`)
VALUES ("Sabato", 6);
INSERT INTO `cinemadb`.`Giorni` (`nome`, `numero`)
VALUES ("Domenica", 7);

COMMIT;
