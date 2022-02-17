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
CREATE SCHEMA IF NOT EXISTS `cinemadb` DEFAULT CHARACTER SET utf8mb4 ;
USE `cinemadb` ;

-- -----------------------------------------------------
-- Table `cinemadb`.`Cinema`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`Cinema` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `indirizzo` VARCHAR(128) NOT NULL,
  `apertura` TIME NOT NULL,
  `chiusura` TIME NOT NULL,
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
-- Table `cinemadb`.`Ruoli`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`Ruoli` (
  `nome` VARCHAR(15) NOT NULL,
  PRIMARY KEY (`nome`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`Dipendenti`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`Dipendenti` (
  `matricola` INT NOT NULL AUTO_INCREMENT,
  `nome` VARCHAR(45) NOT NULL,
  `cognome` VARCHAR(45) NOT NULL,
  `ruolo` VARCHAR(15) NOT NULL,
  PRIMARY KEY (`matricola`),
  INDEX `fk_Dipendenti_Ruoli1_idx` (`ruolo` ASC) VISIBLE,
  CONSTRAINT `fk_Dipendenti_Ruolo1`
    FOREIGN KEY (`ruolo`)
    REFERENCES `cinemadb`.`Ruoli` (`nome`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`Giorni`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`Giorni` (
  `nome` VARCHAR(15) NOT NULL,
  `numero` TINYINT(1) UNSIGNED NOT NULL,
  PRIMARY KEY (`nome`),
  UNIQUE INDEX `ordine_UNIQUE` (`numero` ASC) VISIBLE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`Turni`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`Turni` (
  `dipendente` INT NOT NULL,
  `giorno` VARCHAR(15) NOT NULL,
  `inizio` TIME NOT NULL,
  `durata` TIME NOT NULL,
  `cinema` INT NOT NULL,
  PRIMARY KEY (`dipendente`, `giorno`, `inizio`),
  INDEX `fk_Turni_Cinema1_idx` (`cinema` ASC) VISIBLE,
  INDEX `fk_Turni_Dipendenti1_idx` (`dipendente` ASC) VISIBLE,
  INDEX `fk_Turni_Giorni1_idx` (`giorno` ASC) VISIBLE,
  CONSTRAINT `fk_Turni_Cinema1`
    FOREIGN KEY (`cinema`)
    REFERENCES `cinemadb`.`Cinema` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Turni_Dipendenti1`
    FOREIGN KEY (`dipendente`)
    REFERENCES `cinemadb`.`Dipendenti` (`matricola`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Turni_Giorno1`
    FOREIGN KEY (`giorno`)
    REFERENCES `cinemadb`.`Giorni` (`nome`)
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
-- Table `cinemadb`.`StatiPrenotazione`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`StatiPrenotazione` (
  `nome` VARCHAR(15) NOT NULL,
  PRIMARY KEY (`nome`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`Prenotazioni`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`Prenotazioni` (
  `codice` INT NOT NULL AUTO_INCREMENT,
  `transazione` VARCHAR(256) NOT NULL,
  `cinema` INT NOT NULL,
  `sala` INT NOT NULL,
  `fila` CHAR(1) NOT NULL,
  `numero` INT NOT NULL,
  `data` DATE NOT NULL,
  `ora` TIME NOT NULL,
  `stato` VARCHAR(15) NOT NULL,
  PRIMARY KEY (`codice`),
  UNIQUE INDEX `transazione_UNIQUE` (`transazione` ASC) VISIBLE,
  INDEX `fk_Prenotazioni_Posti1_idx` (`cinema` ASC, `sala` ASC, `fila` ASC, `numero` ASC) VISIBLE,
  INDEX `fk_Prenotazioni_Proiezioni1_idx` (`cinema` ASC, `sala` ASC, `data` ASC, `ora` ASC) VISIBLE,
  INDEX `fk_Prenotazioni_StatiPrenotazione1_idx` (`stato` ASC) VISIBLE,
  CONSTRAINT `fk_Prenotazioni_Posti1`
    FOREIGN KEY (`cinema` , `sala` , `fila` , `numero`)
    REFERENCES `cinemadb`.`Posti` (`cinema` , `sala` , `fila` , `numero`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Prenotazioni_Proiezioni1`
    FOREIGN KEY (`cinema` , `sala` , `data` , `ora`)
    REFERENCES `cinemadb`.`Proiezioni` (`cinema` , `sala` , `data` , `ora`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Prenotazioni_StatoPrenotazione1`
    FOREIGN KEY (`stato`)
    REFERENCES `cinemadb`.`StatiPrenotazione` (`nome`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

USE `cinemadb` ;

-- -----------------------------------------------------
-- procedure mostra_cinema
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `mostra_cinema` ()
BEGIN
	SELECT *
    FROM `Cinema`;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure mostra_proiezioni_cinema
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `mostra_proiezioni_cinema` (IN _cinema_id INT)
BEGIN
	SELECT `cinema`, `sala`, `data`, `ora`, `prezzo`,
		`nome`, `durata`, `casa_cinematografica`, `cast`
    FROM `Proiezioni` JOIN `Film` ON `film` = `id`
    WHERE `cinema` = _cinema_id
		AND (`data` > CURDATE()
			OR (`data` = CURDATE() AND `ora` > TIME(NOW())));
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure mostra_posti_disponibili
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `mostra_posti_disponibili` (
	IN _cinema_id INT,
    IN _sala_id INT,
    IN _data DATE,
    IN _ora TIME)
BEGIN
	SELECT `fila`, `numero`
	FROM `Posti` JOIN `Proiezioni` ON (`Proiezioni`.`cinema` = _cinema_id
										AND `Proiezioni`.`sala` = _sala_id
										AND `Proiezioni`.`data` = _data
										AND `Proiezioni`.`ora` = _ora)
	WHERE (`fila`, `numero`) NOT IN (SELECT `fila`, `numero`
										FROM `Prenotazioni`
										WHERE `Prenotazioni`.`cinema` = _cinema_id
											AND `Prenotazioni`.`sala` = _sala_id
											AND `Prenotazioni`.`data` = _data
											AND `Prenotazioni`.`ora` = _ora
											AND `Prenotazioni`.`stato` != 'Annullata');
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure effettua_prenotazione
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `effettua_prenotazione` (
	IN _cinema_id INT,
    IN _sala_id INT,
    IN _data DATE,
    IN _ora TIME,
    IN _fila CHAR(1),
    IN _numero INT,
    IN _intestatario VARCHAR(128),
    IN _numero_carta NUMERIC(16,0),
    IN _scadenza DATE,
	IN _CVV2 NUMERIC(3))
BEGIN
	# Mock servizio di pagamento---------------------------------------------
    DECLARE tid INT;
    SET tid = (SELECT IFNULL(MAX(`transazione`) + 1, 1) FROM `Prenotazioni`);
	# -----------------------------------------------------------------------
    INSERT INTO `Prenotazioni` (`transazione`, `stato`, `cinema`, `sala`, `data`, `ora`, `fila`, `numero`)
		VALUES (tid, 'Confermata', _cinema_id, _sala_id, _data, _ora, _fila, _numero);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure annulla_prenotazione
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `annulla_prenotazione` (IN _codice INT)
BEGIN
	DECLARE stato_prenotazione VARCHAR(15);
	DECLARE inizio_proiezione TIMESTAMP;
    SET stato_prenotazione = (SELECT `stato` FROM `Prenotazioni` WHERE `codice` = _codice);
    SET inizio_proiezione = (SELECT TIMESTAMP(`data`, `ora`) FROM `Prenotazioni` WHERE `codice` = _codice);
    IF (stato_prenotazione != 'Confermata') THEN
		SIGNAL SQLSTATE '45002'
        SET MESSAGE_TEXT = 'Prenotazione inesistente o invalida.';
    END IF;
    IF (inizio_proiezione > DATE_SUB(NOW(), INTERVAL 30 MINUTE)) THEN
        SIGNAL SQLSTATE '45001'
        SET MESSAGE_TEXT = 'Non è possibile annullare una prenotazione raggiunti i trenta minuti precedenti l\'inizio della proiezione.';
    END IF;
	# Mock servizio di pagamento---------------------------------------------
	# -----------------------------------------------------------------------
    UPDATE `Prenotazioni` SET `stato`='Annullata'
    WHERE `codice` = _codice;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure valida_prenotazione
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `valida_prenotazione` (IN _codice INT)
BEGIN
	DECLARE stato_prenotazione VARCHAR(15);
    SET stato_prenotazione = (SELECT `stato` FROM `Prenotazioni` WHERE `codice` = _codice);
    IF (stato_prenotazione != 'Confermata') THEN
		SIGNAL SQLSTATE '45002'
        SET MESSAGE_TEXT = 'Prenotazione inesistente o invalida.';
    END IF;
    UPDATE `Prenotazioni` SET `stato`='Validata'
    WHERE `codice` = _codice;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure mostra_proiezioni_senza_proiezionista
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `mostra_proiezioni_senza_proiezionista` ()
BEGIN
	SELECT *
    FROM `Proiezioni`
    WHERE `proiezionista` IS NULL
		AND `data` > CURDATE()
        OR (`data` = CURDATE() AND `ora` > TIME(NOW()));
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure mostra_cinema_senza_maschere
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `mostra_cinema_senza_maschere` ()
BEGIN
WITH
    calendario_cinema AS (SELECT `id` AS cinema, `nome` AS giorno,
                                 `apertura`, `chiusura`
						  FROM `Cinema` CROSS JOIN `Giorni`),
    eventi AS (SELECT `cinema`, `giorno`, `apertura` AS `ora`, 0 AS `variazione`
               FROM calendario_cinema
               UNION ALL
               SELECT `cinema`, `giorno`, `inizio`, 1
               FROM `Turni` JOIN `Dipendenti` ON `dipendente` = `matricola`
               WHERE `ruolo` = 'Maschera'
               UNION ALL
               SELECT `cinema`, `giorno`,
                      SEC_TO_TIME(TIME_TO_SEC(`inizio`) + TIME_TO_SEC(`durata`)), -1
               FROM `Turni` JOIN `Dipendenti` ON `dipendente` = `matricola`
               WHERE `ruolo` = 'Maschera'
               UNION ALL
               SELECT `cinema`, `giorno`, `chiusura`, 0 FROM calendario_cinema),
    invervalli_eventi AS (SELECT `cinema`, `giorno`, `ora` AS `dalle_ore`,
                                 LEAD(`ora`) OVER (PARTITION BY `cinema`, `giorno`
                                                 ORDER BY `ora`) AS `alle_ore`,
                                 SUM(`variazione`) OVER (PARTITION BY `cinema`, `giorno`
                                                 ORDER BY `ora`) AS numero_maschere
                          FROM eventi),
    gruppi_variazioni AS (SELECT `cinema`, `giorno`, `dalle_ore`, `alle_ore`,
                                 `numero_maschere`,
                                 CASE WHEN `numero_maschere` != lag(`numero_maschere`)
                                           OVER (PARTITION BY `cinema`, `giorno`
                                                 ORDER BY `dalle_ore`, `alle_ore`)
                                      THEN 1
                                      ELSE 0
                                 END AS variazione
                          FROM invervalli_eventi),
    gruppi_orari AS (SELECT `cinema`, `giorno`, `dalle_ore`, `alle_ore`, `numero_maschere`,
                            sum(`variazione`) OVER (PARTITION BY `cinema`, `giorno`
                                                  ORDER BY `dalle_ore`, `alle_ore`)
                                                  AS `gruppo`
                     FROM gruppi_variazioni)
SELECT `cinema`, `giorno`, min(`dalle_ore`) AS `dalle_ore`,
       max(`alle_ore`) AS `alle_ore`, `numero_maschere`
FROM gruppi_orari
WHERE `dalle_ore` != `alle_ore`
GROUP BY `cinema`, `giorno`, `numero_maschere`, `gruppo`
HAVING `numero_maschere` < 2
ORDER BY `cinema`, NUMERO_GIORNO(`giorno`), min(`dalle_ore`);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure mostra_stato_prenotazioni
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `mostra_stato_prenotazioni` ()
BEGIN
	SELECT `Sale`.`cinema`, `Sale`.`numero` AS sala,
		IFNULL(SUM(`stato` = 'Confermata'
			OR `stato` = 'Validata'
            OR `stato` = 'Scaduta'), 0) AS confermate,
		IFNULL(SUM(`stato` = 'Annullata'), 0) AS annullate,
		IFNULL(SUM(`stato` = 'Validata'), 0) AS validate,
		IFNULL(SUM(`stato` = 'Scaduta'), 0) AS scadute
	FROM `Prenotazioni` RIGHT OUTER JOIN `Sale`
		ON `Prenotazioni`.`cinema` = `Sale`.`cinema`
			AND `sala` = `Sale`.`numero`
    GROUP BY `Sale`.`cinema`, `Sale`.`numero`;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserisci_proiezione
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `inserisci_proiezione` (
	IN _cinema INT,
	IN _sala INT,
	IN _data DATE,
	IN _ora TIME,
	IN _prezzo NUMERIC(15,2),
	IN _film INT)
BEGIN
	INSERT INTO `Proiezioni` (`cinema`, `sala`, `data`, `ora`, `prezzo`, `film`)
    VALUES (_cinema, _sala, _data, _ora, _prezzo, _film);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure assegna_proiezionista
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `assegna_proiezionista` (
	IN _proiezionista INT,
    IN _cinema INT,
	IN _sala INT,
	IN _data DATE,
	IN _ora TIME)
BEGIN
	UPDATE `Proiezioni`
    SET `proiezionista` = _proiezionista
    WHERE `cinema` = _cinema AND `sala` = _sala
		AND `data` = _data AND `ora` = _ora;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserisci_turno
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `inserisci_turno` (
	IN _dipendente INT,
	IN _giorno VARCHAR(15),
	IN _inizio TIME,
	IN _durata TIME,
	IN _cinema INT)
BEGIN
	INSERT INTO `Turni` (`dipendente`, `giorno`, `inizio`, `durata`, `cinema`)
    VALUES (_dipendente, _giorno, _inizio, _durata, _cinema);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure elimina_turno
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `elimina_turno` (
	IN _dipendente INT,
	IN _giorno VARCHAR(15),
	IN _inizio TIME)
BEGIN
	DELETE FROM `Turni`
    WHERE `dipendente` = _dipendente
		AND `giorno` = _giorno
		AND `inizio` = _inizio;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure mostra_proiezioni
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `mostra_proiezioni` ()
BEGIN
	SELECT `cinema`, `sala`, `data`, `ora`, `prezzo`, `id`, `nome`, `durata`
    FROM `Proiezioni` JOIN `Film` ON `film` = `id`
    WHERE `data` > CURDATE() OR (`data` = CURDATE() AND `ora` > TIME(NOW()));
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure elimina_proiezione
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `elimina_proiezione` (
    IN _cinema INT,
	IN _sala INT,
	IN _data DATE,
	IN _ora TIME)
BEGIN
	DELETE FROM `Proiezioni`
    WHERE `cinema` = _cinema AND `sala` = _sala
		AND `data` = _data AND `ora` = _ora;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure mostra_turni
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `mostra_turni` ()
BEGIN
	SELECT `giorno`, `inizio`, `durata`, `cinema`,
		`matricola`, `nome`, `cognome`, `ruolo`
    FROM `Turni` JOIN `Dipendenti` WHERE `dipendente` = `matricola`;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- function GIORNO_DELLA_SETTIMANA
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE FUNCTION `GIORNO_DELLA_SETTIMANA` (_data DATE)
RETURNS VARCHAR(15)
DETERMINISTIC
READS SQL DATA
BEGIN
	RETURN (SELECT `nome`
			FROM `Giorni`
            WHERE `numero` = (MOD(MOD(DAYOFWEEK(_data) - 2, 7) + 7, 7) + 1));
END$$

DELIMITER ;

-- -----------------------------------------------------
-- function NUMERO_GIORNO
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE FUNCTION `NUMERO_GIORNO` (_nome VARCHAR(15))
RETURNS TINYINT
DETERMINISTIC
READS SQL DATA
BEGIN
	RETURN (SELECT `numero`
			FROM `Giorni`
            WHERE `nome` = _nome);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure mostra_proiezionisti_disponibili
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `mostra_proiezionisti_disponibili` (
	IN _cinema INT,
	IN _sala INT,
	IN _data DATE,
	IN _ora TIME)
BEGIN
	SELECT matricola, nome, cognome
    FROM Dipendente JOIN Turni ON matricola = dipendente
		JOIN Proiezione ON Proiezione.cinema = _cinema AND Proiezione.sala = _sala AND Proiezione.data = _data AND Proiezione._ora = _ora
		JOIN Film ON id = Proiezioni.film
    WHERE inizio <= _ora AND Turni.durata >= Film.durata;
END$$

DELIMITER ;
USE `cinemadb`;

DELIMITER $$
USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Cinema_BEFORE_INSERT`
BEFORE INSERT ON `Cinema`
FOR EACH ROW
BEGIN
	IF (NEW.`chiusura` < NEW.`apertura`) THEN
		SIGNAL SQLSTATE '45001'
		SET MESSAGE_TEXT = 'L\'orario di chiusura non può precedere quello di apertura';
    END IF;
END$$

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
CREATE TRIGGER `cinemadb`.`Turni_BEFORE_INSERT_3`
BEFORE INSERT ON `Turni`
FOR EACH ROW
BEGIN
	DECLARE ora_apertura_cinema TIME;
    DECLARE ora_chiusura_cinema TIME;
    SET ora_apertura_cinema = (SELECT `apertura` FROM `Cinema` WHERE `id` = NEW.`cinema`);
    SET ora_chiusura_cinema = (SELECT `chiusura` FROM `Cinema` WHERE `id` = NEW.`cinema`);
    IF (NEW.`inizio` < ora_apertura_cinema
		OR SEC_TO_TIME(TIME_TO_SEC(NEW.`inizio`) + TIME_TO_SEC(NEW.`durata`)) > ora_chiusura_cinema) THEN
        SIGNAL SQLSTATE '45001'
        SET MESSAGE_TEXT = 'Il cinema selezionato è chiuso nell\'intervallo di tempo specificato.';
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

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_INSERT_1`
BEFORE INSERT ON `Proiezioni`
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
        SIGNAL SQLSTATE '45001'
        SET MESSAGE_TEXT = 'Il cinema selezionato è chiuso nell\'intervallo di tempo specificato.';
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_INSERT_2`
BEFORE INSERT ON `Proiezioni`
FOR EACH ROW
BEGIN
	DECLARE ruolo VARCHAR(15);
    SET ruolo = (SELECT `ruolo` FROM `Dipendenti` WHERE `matricola` = NEW.`proiezionista`);
    IF (ruolo != 'Proiezionista') THEN
        SIGNAL SQLSTATE '45001'
        SET MESSAGE_TEXT = 'Impossibile assegnare ad una proiezione un dipendente non proiezionista.';
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_INSERT_3`
BEFORE INSERT ON `Proiezioni`
FOR EACH ROW
BEGIN
DECLARE esiste_turno BOOL;
SET esiste_turno = (SELECT COUNT(*)
					FROM Proiezioni JOIN Film ON film = id 
									JOIN Turni ON Proiezioni.cinema = Turni.cinema
												AND GIORNO_DELLA_SETTIMANA(Proiezioni.data) = Turni.giorno
												AND Proiezioni.proiezionista = Turni.dipendente
					WHERE Proiezioni.cinema = NEW.cinema
						AND Proiezioni.sala = NEW.sala
						AND Proiezioni.data = NEW.data
						AND Proiezioni.ora = NEW.ora
                        AND Proiezioni.film = NEW.film
						AND Proiezioni.proiezionista = NEW.proiezionista
						AND inizio <= ora
						AND SEC_TO_TIME(TIME_TO_SEC(ora) + TIME_TO_SEC(Film.durata))
						<= SEC_TO_TIME(TIME_TO_SEC(inizio) + TIME_TO_SEC(Turni.durata)));
IF (esiste_turno = FALSE) THEN
        SIGNAL SQLSTATE '45001'
        SET MESSAGE_TEXT = 'Il proiezionista selezionato non è assegnato ad un turno compatibile con la proiezione.';
END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_UPDATE`
BEFORE UPDATE ON `Proiezioni`
FOR EACH ROW
BEGIN
	DECLARE ruolo VARCHAR(15);
    SET ruolo = (SELECT `ruolo` FROM `Dipendenti` WHERE `matricola` = NEW.`proiezionista`);
    IF (ruolo != 'Proiezionista') THEN
        SIGNAL SQLSTATE '45001'
        SET MESSAGE_TEXT = 'Impossibile assegnare ad una proiezione un dipendente non proiezionista.';
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_UPDATE_1`
BEFORE UPDATE ON `Proiezioni`
FOR EACH ROW
BEGIN
DECLARE esiste_turno BOOL;
SET esiste_turno = (SELECT COUNT(*)
					FROM Proiezioni JOIN Film ON film = id 
									JOIN Turni ON Proiezioni.cinema = Turni.cinema
												AND GIORNO_DELLA_SETTIMANA(Proiezioni.data) = Turni.giorno
												AND Proiezioni.proiezionista = Turni.dipendente
					WHERE Proiezioni.cinema = NEW.cinema
						AND Proiezioni.sala = NEW.sala
						AND Proiezioni.data = NEW.data
						AND Proiezioni.ora = NEW.ora
						AND Proiezioni.proiezionista = NEW.proiezionista
						AND inizio <= ora
						AND SEC_TO_TIME(TIME_TO_SEC(ora) + TIME_TO_SEC(Film.durata))
						<= SEC_TO_TIME(TIME_TO_SEC(inizio) + TIME_TO_SEC(Turni.durata)));
IF (esiste_turno = FALSE) THEN
        SIGNAL SQLSTATE '45001'
        SET MESSAGE_TEXT = 'Il proiezionista selezionato non è assegnato ad un turno compatibile con la proiezione.';
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
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (1, 'Via Giuseppe Verdi 72100 BR Brindisi', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (2, 'Via Roma 12100 CN Cremona', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (3, 'Via Dante 40100 BO Bologna', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (4, 'Via Cavour 1810 IM Imperia', '16:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (5, 'Via Monte Bianco 94100 EN Enna', '16:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (6, 'Via Giove 28100 NO Novara', '16:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (7, 'Via della Repubblica RM 00100 Roma', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (8, 'Via de Gasperi 33170 PN Pordenone', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (9, 'Via dei Tulipani 37100 VR Verona', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (10, 'Via dei Frati 92100 AO Aosta', '10:00:00', '23:00:00');

COMMIT;


-- -----------------------------------------------------
-- Data for table `cinemadb`.`Ruoli`
-- -----------------------------------------------------
START TRANSACTION;
USE `cinemadb`;
INSERT INTO `cinemadb`.`Ruoli` (`nome`) VALUES ('Proiezionista');
INSERT INTO `cinemadb`.`Ruoli` (`nome`) VALUES ('Maschera');

COMMIT;


-- -----------------------------------------------------
-- Data for table `cinemadb`.`Giorni`
-- -----------------------------------------------------
START TRANSACTION;
USE `cinemadb`;
INSERT INTO `cinemadb`.`Giorni` (`nome`, `numero`) VALUES ('Lunedì', 1);
INSERT INTO `cinemadb`.`Giorni` (`nome`, `numero`) VALUES ('Martedì', 2);
INSERT INTO `cinemadb`.`Giorni` (`nome`, `numero`) VALUES ('Mercoledì', 3);
INSERT INTO `cinemadb`.`Giorni` (`nome`, `numero`) VALUES ('Giovedì', 4);
INSERT INTO `cinemadb`.`Giorni` (`nome`, `numero`) VALUES ('Venerdì', 5);
INSERT INTO `cinemadb`.`Giorni` (`nome`, `numero`) VALUES ('Sabato', 6);
INSERT INTO `cinemadb`.`Giorni` (`nome`, `numero`) VALUES ('Domenica', 7);

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


-- -----------------------------------------------------
-- Data for table `cinemadb`.`StatiPrenotazione`
-- -----------------------------------------------------
START TRANSACTION;
USE `cinemadb`;
INSERT INTO `cinemadb`.`StatiPrenotazione` (`nome`) VALUES ('Confermata');
INSERT INTO `cinemadb`.`StatiPrenotazione` (`nome`) VALUES ('Validata');
INSERT INTO `cinemadb`.`StatiPrenotazione` (`nome`) VALUES ('Scaduta');
INSERT INTO `cinemadb`.`StatiPrenotazione` (`nome`) VALUES ('Annullata');

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
-- begin attached script 'Popolazione'
-- -----------------------------------------------------
-- Data for table `cinemadb`.`Sale`
-- -----------------------------------------------------

INSERT INTO `Sale` (`cinema`, `numero`)
WITH RECURSIVE numeri AS (SELECT 1 AS numero
						  UNION ALL
						  SELECT numero + 1
						  FROM numeri
						  WHERE numero + 1 <= 5)
SELECT id, numero
FROM cinema CROSS JOIN numeri;

-- -----------------------------------------------------
-- Data for table `cinemadb`.`Posti`
-- -----------------------------------------------------

INSERT INTO `Posti` (`cinema`, `sala`, `fila`, `numero`)
WITH RECURSIVE numeri AS (SELECT 1 AS numero
						  UNION ALL
						  SELECT numero + 1
						  FROM numeri
						  WHERE numero + 1 <= 25),
	lettere AS (SELECT 'A' AS lettera
						  UNION ALL
						  SELECT CHAR(ASCII(lettera) + 1 USING ASCII)
						  FROM lettere
						  WHERE ASCII(lettera) + 1 <= ASCII('A') + 8)
SELECT cinema, Sale.numero, lettera, numeri.numero
FROM Sale CROSS JOIN lettere CROSS JOIN numeri;

-- -----------------------------------------------------
-- Data for table `cinemadb`.`Dipendenti`
-- -----------------------------------------------------

INSERT INTO `Dipendenti` (`matricola`, `nome`, `cognome`, `ruolo`)
WITH RECURSIVE matricole AS (SELECT 1 AS matricola
							 UNION ALL
							 SELECT matricola + 1
							 FROM matricole
							 WHERE matricola + 1 <= 200)
SELECT matricola, 'Mario', 'Rossi',
		CASE WHEN matricola <= 50
        THEN 'Maschera'
        ELSE 'Proiezionista'
        END
FROM matricole;

-- -----------------------------------------------------
-- Data for table `cinemadb`.`Turni`
-- -----------------------------------------------------

INSERT INTO `Turni` (`dipendente`, `giorno`, `inizio`, `durata`, `cinema`) VALUES (2, 'Lunedì', '10:00:00', '01:00:00', 1);
INSERT INTO `Turni` (`dipendente`, `giorno`, `inizio`, `durata`, `cinema`) VALUES (2, 'Martedì', '10:00:00', '08:00:00', 1);
INSERT INTO `Turni` (`dipendente`, `giorno`, `inizio`, `durata`, `cinema`) VALUES (4, 'Lunedì', '10:30:00', '01:30:00', 1);
INSERT INTO `Turni` (`dipendente`, `giorno`, `inizio`, `durata`, `cinema`) VALUES (4, 'Lunedì', '16:30:00', '01:00:00', 1);
INSERT INTO `Turni` (`dipendente`, `giorno`, `inizio`, `durata`, `cinema`) VALUES (4, 'Martedì', '18:00:00', '05:00:00', 1);
INSERT INTO `Turni` (`dipendente`, `giorno`, `inizio`, `durata`, `cinema`) VALUES (6, 'Lunedì', '10:45:00', '00:30:00', 1);
INSERT INTO `Turni` (`dipendente`, `giorno`, `inizio`, `durata`, `cinema`) VALUES (6, 'Martedì', '10:00:00', '08:00:00', 1);
INSERT INTO `Turni` (`dipendente`, `giorno`, `inizio`, `durata`, `cinema`) VALUES (7, 'Lunedì', '16:00:00', '02:00:00', 1);
INSERT INTO `Turni` (`dipendente`, `giorno`, `inizio`, `durata`, `cinema`) VALUES (7, 'Martedì', '18:00:00', '05:00:00', 1);

-- end attached script 'Popolazione'
