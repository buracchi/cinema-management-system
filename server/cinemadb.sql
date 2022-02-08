-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema cinemadb
-- -----------------------------------------------------
DROP SCHEMA IF EXISTS `cinemadb` ;

-- -----------------------------------------------------
-- Schema cinemadb
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `cinemadb` DEFAULT CHARACTER SET utf8 ;
USE `cinemadb` ;

-- -----------------------------------------------------
-- Table `cinemadb`.`Cinema`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`Cinema` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `indirizzo` VARCHAR(128) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `indirizzo_UNIQUE` (`indirizzo` ASC) VISIBLE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`Sale`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`Sale` (
  `cinema` INT NOT NULL,
  `numero` INT NOT NULL,
  PRIMARY KEY (`cinema`, `numero`),
  INDEX `fk_Sale_Cinema_idx` (`cinema` ASC) VISIBLE,
  CONSTRAINT `fk_Sale_Cinema`
    FOREIGN KEY (`cinema`)
    REFERENCES `cinemadb`.`Cinema` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`Posti`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`Posti` (
  `cinema` INT NOT NULL,
  `sala` INT NOT NULL,
  `fila` CHAR(1) NOT NULL,
  `numero` INT NOT NULL,
  PRIMARY KEY (`cinema`, `sala`, `fila`, `numero`),
  CONSTRAINT `fk_Posti_Sale1`
    FOREIGN KEY (`cinema` , `sala`)
    REFERENCES `cinemadb`.`Sale` (`cinema` , `numero`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`Dipendenti`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`Dipendenti` (
  `matricola` INT NOT NULL AUTO_INCREMENT,
  `nome` VARCHAR(45) NOT NULL,
  `cognome` VARCHAR(45) NOT NULL,
  `ruolo` ENUM('maschera', 'proiezionista') NOT NULL,
  PRIMARY KEY (`matricola`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`Turni`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`Turni` (
  `dipendente` INT NOT NULL,
  `giorno` ENUM('lun', 'mar', 'mer', 'gio', 'ven', 'sab', 'dom') NOT NULL,
  `inizio` TIME NOT NULL,
  `durata` TIME NOT NULL,
  `cinema` INT NOT NULL,
  PRIMARY KEY (`dipendente`, `giorno`, `inizio`),
  INDEX `fk_Turni_Cinema1_idx` (`cinema` ASC) VISIBLE,
  INDEX `fk_Turni_Dipendenti1_idx` (`dipendente` ASC) VISIBLE,
  CONSTRAINT `fk_Turni_Cinema1`
    FOREIGN KEY (`cinema`)
    REFERENCES `cinemadb`.`Cinema` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Turni_Dipendenti1`
    FOREIGN KEY (`dipendente`)
    REFERENCES `cinemadb`.`Dipendenti` (`matricola`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`Film`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`Film` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `nome` VARCHAR(45) NOT NULL,
  `durata` TIME NOT NULL,
  `casa_cinematografica` VARCHAR(45) NULL,
  `cast` VARCHAR(1024) NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`Proiezioni`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`Proiezioni` (
  `cinema` INT NOT NULL,
  `sala` INT NOT NULL,
  `data` DATE NOT NULL,
  `ora` TIME NOT NULL,
  `prezzo` DECIMAL(15,2) NOT NULL,
  `film` INT NOT NULL,
  `proiezionista` INT NULL,
  PRIMARY KEY (`cinema`, `sala`, `data`, `ora`),
  INDEX `fk_Proiezioni_Film1_idx` (`film` ASC) VISIBLE,
  INDEX `fk_Proiezioni_Dipendenti1_idx` (`proiezionista` ASC) VISIBLE,
  CONSTRAINT `fk_Proiezioni_Sale1`
    FOREIGN KEY (`cinema` , `sala`)
    REFERENCES `cinemadb`.`Sale` (`cinema` , `numero`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Proiezioni_Film1`
    FOREIGN KEY (`film`)
    REFERENCES `cinemadb`.`Film` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Proiezioni_Dipendenti1`
    FOREIGN KEY (`proiezionista`)
    REFERENCES `cinemadb`.`Dipendenti` (`matricola`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`Prenotazioni`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`Prenotazioni` (
  `codice` INT NOT NULL AUTO_INCREMENT,
  `transazione` VARCHAR(256) NOT NULL,
  `stato` ENUM('confermata', 'annullata', 'validata', 'scaduta') NOT NULL,
  `cinema` INT NOT NULL,
  `sala` INT NOT NULL,
  `fila` CHAR(1) NOT NULL,
  `numero` INT NOT NULL,
  `data` DATE NOT NULL,
  `ora` TIME NOT NULL,
  PRIMARY KEY (`codice`),
  UNIQUE INDEX `transazione_UNIQUE` (`transazione` ASC) VISIBLE,
  INDEX `fk_Prenotazioni_Posti1_idx` (`cinema` ASC, `sala` ASC, `fila` ASC, `numero` ASC) VISIBLE,
  INDEX `fk_Prenotazioni_Proiezioni1_idx` (`cinema` ASC, `sala` ASC, `data` ASC, `ora` ASC) VISIBLE,
  CONSTRAINT `fk_Prenotazioni_Posti1`
    FOREIGN KEY (`cinema` , `sala` , `fila` , `numero`)
    REFERENCES `cinemadb`.`Posti` (`cinema` , `sala` , `fila` , `numero`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Prenotazioni_Proiezioni1`
    FOREIGN KEY (`cinema` , `sala` , `data` , `ora`)
    REFERENCES `cinemadb`.`Proiezioni` (`cinema` , `sala` , `data` , `ora`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

USE `cinemadb` ;

-- -----------------------------------------------------
-- Placeholder table for view `cinemadb`.`OrariCinema`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`OrariCinema` (`id` INT);

-- -----------------------------------------------------
-- procedure popola
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `popola` ()
BEGIN
	
END$$

DELIMITER ;

-- -----------------------------------------------------
-- View `cinemadb`.`OrariCinema`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `cinemadb`.`OrariCinema`;
USE `cinemadb`;
SELECT `cinema`, `data`, MIN(`ora`) AS `inizio`,
	SEC_TO_TIME(MAX(TIME_TO_SEC(`ora`) + TIME_TO_SEC(`durata`))) AS `fine`
FROM `Proiezioni` JOIN `Film` ON `id` = `film`
GROUP BY `cinema`, `data`;
USE `cinemadb`;

DELIMITER $$
USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Turni_BEFORE_INSERT`
BEFORE INSERT ON `Turni`
FOR EACH ROW
BEGIN
    IF (NEW.`durata` > TIME('08:00:00')) THEN
        SIGNAL SQLSTATE '45001'
        SET MESSAGE_TEXT = 'Impossibile creare un turno di più di 8 ore.';
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Turni_BEFORE_INSERT_1` 
BEFORE INSERT ON `Turni` 
FOR EACH ROW
BEGIN
	DECLARE OreTotali TIME;
    SET OreTotali = (SELECT SEC_TO_TIME(SUM(TIME_TO_SEC(`Turni`.`durata`)))
                        FROM `Turni`
                        WHERE `Turni`.`dipendente` = NEW.`dipendente`
                            AND `Turni`.`giorno` = NEW.`giorno`);
    IF (OreTotali + NEW.`durata` > TIME('08:00:00')) THEN
        SIGNAL SQLSTATE '45001'
        SET MESSAGE_TEXT = 'La somma della durata dei turni nella giornata supera le 8 ore.';
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Turni_BEFORE_INSERT_2`
BEFORE INSERT ON `Turni`
FOR EACH ROW
BEGIN
    DECLARE clash BOOL;
    SET clash = EXISTS (SELECT * 
                        FROM `Turni`
                        WHERE `Turni`.`dipendente` = NEW.`dipendente`
                            AND `Turni`.`giorno` = NEW.`giorno`
                            AND `Turni`.`inizio` < 
                            (SEC_TO_TIME(TIME_TO_SEC(NEW.`inizio`) 
                            + TIME_TO_SEC(NEW.`durata`))) 
                            AND (SEC_TO_TIME(TIME_TO_SEC(`Turni`.`inizio`) 
                            + TIME_TO_SEC(`Turni`.`durata`))) > NEW.`inizio`);
    IF (clash) THEN
        SIGNAL SQLSTATE '45001'
        SET MESSAGE_TEXT = 'Il dipendente è già assegnato ad un turno nell\'arco temporale selezionato.';
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_INSERT`
BEFORE INSERT ON `Proiezioni`
FOR EACH ROW
BEGIN
	DECLARE inizio_proiezione TIMESTAMP;
	DECLARE fine_proiezione TIMESTAMP;
	DECLARE fine_proiezione_prec TIMESTAMP;
	DECLARE inizio_proiezione_succ TIMESTAMP;
    SET inizio_proiezione = TIMESTAMP(NEW.`data`, NEW.`ora`);
    SET fine_proiezione = TIMESTAMP(NEW.`data`, NEW.`ora` + (SELECT `durata` FROM `Film` WHERE `id` = NEW.`film`));
	SET inizio_proiezione_succ = (SELECT TIMESTAMP(`data`, `ora`)
										FROM `Proiezioni`
										WHERE `cinema` = NEW.`cinema` AND `sala` = NEW.`sala`
											AND (`data` > NEW.`data` OR (`data` = NEW.`data` AND `ora` > NEW.`ora`))
										ORDER BY `data` ASC, `ora` ASC
										LIMIT 1);
	SET fine_proiezione_prec = (SELECT TIMESTAMP(`data`, `ora` + `durata`)
										FROM `Proiezioni` JOIN `Film` on `id` = `film`
										WHERE `cinema` = NEW.`cinema` AND `sala` = NEW.`sala`
											AND (`data` < NEW.`data` OR (`data` = NEW.`data` AND `ora` <= NEW.`ora`))
										ORDER BY `data` DESC, `ora` DESC
										LIMIT 1);
    IF ((fine_proiezione_prec IS NOT NULL AND fine_proiezione_prec > inizio_proiezione)
		OR (inizio_proiezione_succ IS NOT NULL AND inizio_proiezione_succ < fine_proiezione)) THEN
		SIGNAL SQLSTATE '45001'
		SET MESSAGE_TEXT = 'La sala selezionata è già impegnata in una proiezione nell\'orario selezionato.';
    END IF;
END$$


DELIMITER ;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

-- -----------------------------------------------------
-- Data for table `cinemadb`.`Cinema`
-- -----------------------------------------------------
START TRANSACTION;
USE `cinemadb`;
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`) VALUES (DEFAULT, 'Via Giuseppe Verdi 72100 BR Brindisi');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`) VALUES (DEFAULT, 'Via Roma 12100 CN Cremona');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`) VALUES (DEFAULT, 'Via Dante 40100 BO Bologna');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`) VALUES (DEFAULT, 'Via Cavour 1810 IM Imperia');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`) VALUES (DEFAULT, 'Via Monte Bianco 94100 EN Enna');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`) VALUES (DEFAULT, 'Via Giove 28100 NO Novara');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`) VALUES (DEFAULT, 'Via della Repubblica RM 00100 Roma');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`) VALUES (DEFAULT, 'Via de Gasperi 33170 PN Pordenone');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`) VALUES (DEFAULT, 'Via dei Tulipani 37100 VR Verona');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`) VALUES (DEFAULT, 'Via dei Frati 92100 AO Aosta');

COMMIT;


-- -----------------------------------------------------
-- Data for table `cinemadb`.`Sale`
-- -----------------------------------------------------
START TRANSACTION;
USE `cinemadb`;
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (1, 1);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (1, 2);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (1, 3);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (1, 4);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (1, 5);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (2, 1);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (2, 2);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (2, 3);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (3, 1);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (3, 2);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (3, 3);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (3, 4);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (4, 1);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (4, 2);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (4, 3);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (4, 4);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (4, 5);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (4, 6);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (4, 7);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (5, 1);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (5, 2);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (5, 3);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (5, 4);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (5, 5);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (6, 1);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (6, 2);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (6, 3);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (6, 4);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (6, 5);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (7, 1);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (7, 2);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (7, 3);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (7, 4);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (7, 5);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (7, 6);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (8, 1);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (8, 2);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (8, 3);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (9, 1);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (9, 2);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (9, 3);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (9, 4);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (9, 5);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (9, 6);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (9, 7);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (10, 1);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (10, 2);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (10, 3);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (10, 4);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (10, 5);
INSERT INTO `cinemadb`.`Sale` (`cinema`, `numero`) VALUES (10, 6);

COMMIT;


-- -----------------------------------------------------
-- Data for table `cinemadb`.`Dipendenti`
-- -----------------------------------------------------
START TRANSACTION;
USE `cinemadb`;
INSERT INTO `cinemadb`.`Dipendenti` (`matricola`, `nome`, `cognome`, `ruolo`) VALUES (DEFAULT, 'Mario', 'Rossi', 'proiezionista');
INSERT INTO `cinemadb`.`Dipendenti` (`matricola`, `nome`, `cognome`, `ruolo`) VALUES (DEFAULT, 'Giuseppe', 'Verdi', 'maschera');
INSERT INTO `cinemadb`.`Dipendenti` (`matricola`, `nome`, `cognome`, `ruolo`) VALUES (DEFAULT, 'Luca', 'Bianchi', 'proiezionista');
INSERT INTO `cinemadb`.`Dipendenti` (`matricola`, `nome`, `cognome`, `ruolo`) VALUES (DEFAULT, 'Gino', 'Bruni', 'maschera');
INSERT INTO `cinemadb`.`Dipendenti` (`matricola`, `nome`, `cognome`, `ruolo`) VALUES (DEFAULT, 'Valerio', 'Amico', 'proiezionista');
INSERT INTO `cinemadb`.`Dipendenti` (`matricola`, `nome`, `cognome`, `ruolo`) VALUES (DEFAULT, 'Maria', 'Marrone', 'maschera');
INSERT INTO `cinemadb`.`Dipendenti` (`matricola`, `nome`, `cognome`, `ruolo`) VALUES (DEFAULT, 'Ludovica', 'Cortese', 'proiezionista');

COMMIT;


-- -----------------------------------------------------
-- Data for table `cinemadb`.`Film`
-- -----------------------------------------------------
START TRANSACTION;
USE `cinemadb`;
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Il padrino', '02:55:00', '', NULL);
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Il cavaliere oscuro', '02:32:00', NULL, NULL);
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Schindler\'s List', '03:15:00', NULL, NULL);
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Pulp Fiction', '02:34:00', NULL, NULL);
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Il buono, il brutto, il cattivo', '02:41:00', NULL, NULL);
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Fight Club', '02:19:00', NULL, NULL);
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Matrix', '02:16:00', NULL, NULL);
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Amici miei', '02:20:00', NULL, NULL);

COMMIT;

-- begin attached script 'Eventi'
SET GLOBAL event_scheduler = ON;

DROP EVENT IF EXISTS `cinemadb`.`pulizia_prenotazioni`;
CREATE EVENT `cinemadb`.`pulizia_prenotazioni`
ON SCHEDULE EVERY 1 MONTH
STARTS TIMESTAMP(DATE_FORMAT(DATE_ADD(CURDATE(), INTERVAL 1 MONTH), '%Y-%m-1'))
DO
	DELETE FROM `Prenotazioni` WHERE `data` < CURDATE();
    
DROP EVENT IF EXISTS `cinemadb`.`pulizia_proiezioni`;
CREATE EVENT `cinemadb`.`pulizia_proiezioni`
ON SCHEDULE EVERY 1 DAY
STARTS TIMESTAMP(CURDATE())
DO
	DELETE FROM `Proiezioni` WHERE `data` < CURDATE();
-- end attached script 'Eventi'
