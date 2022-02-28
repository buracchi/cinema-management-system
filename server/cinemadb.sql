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
    ON DELETE CASCADE
    ON UPDATE CASCADE)
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
    ON DELETE CASCADE
    ON UPDATE CASCADE)
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
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Turni_Dipendenti1`
    FOREIGN KEY (`dipendente`)
    REFERENCES `cinemadb`.`Dipendenti` (`matricola`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
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
  `casa_cinematografica` VARCHAR(256) NULL,
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
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Proiezioni_Film1`
    FOREIGN KEY (`film`)
    REFERENCES `cinemadb`.`Film` (`id`)
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Proiezioni_Dipendenti1`
    FOREIGN KEY (`proiezionista`)
    REFERENCES `cinemadb`.`Dipendenti` (`matricola`)
    ON DELETE SET NULL
    ON UPDATE CASCADE)
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
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Prenotazioni_Proiezioni1`
    FOREIGN KEY (`cinema` , `sala` , `data` , `ora`)
    REFERENCES `cinemadb`.`Proiezioni` (`cinema` , `sala` , `data` , `ora`)
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Prenotazioni_StatoPrenotazione1`
    FOREIGN KEY (`stato`)
    REFERENCES `cinemadb`.`StatiPrenotazione` (`nome`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

USE `cinemadb` ;

-- -----------------------------------------------------
-- Placeholder table for view `cinemadb`.`Palinsesti`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cinemadb`.`Palinsesti` (`data` INT, `ora` INT, `cinema` INT, `sala` INT, `prezzo` INT, `movie_id` INT, `nome` INT, `durata` INT, `casa_cinematografica` INT, `cast` INT);

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
-- procedure mostra_palinsesto
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `mostra_palinsesto` (IN _cinema_id INT)
BEGIN
	SELECT *
    FROM `Palinsesti`
    WHERE `cinema` = _cinema_id;
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
    UPDATE `Prenotazioni` SET `stato`='Annullata'
    WHERE `codice` = _codice;
	# Mock servizio di pagamento---------------------------------------------
	# -----------------------------------------------------------------------
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure valida_prenotazione
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `valida_prenotazione` (IN _codice INT)
BEGIN
    UPDATE `Prenotazioni` SET `stato` = 'Validata'
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
SELECT `cinema`, `giorno`, MIN(`dalle_ore`) AS `dalle_ore`,
       MAX(`alle_ore`) AS `alle_ore`, `numero_maschere`
FROM gruppi_orari
WHERE `dalle_ore` != `alle_ore`
GROUP BY `cinema`, `giorno`, `numero_maschere`, `gruppo`
HAVING `numero_maschere` < 2
ORDER BY `cinema`, NUMERO_GIORNO(`giorno`), MIN(`dalle_ore`);
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
	SELECT *
    FROM `Proiezioni` JOIN `Film` ON `film` = `id`
    WHERE `data` > CURDATE()
		OR (`data` = CURDATE() AND `ora` > TIME(NOW()))
	ORDER BY `data`, `ora`, `cinema`, `sala`;
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
	DECLARE _fine TIME;
    SET _fine = (SELECT SEC_TO_TIME(TIME_TO_SEC(`ora`) + TIME_TO_SEC(`durata`))
					FROM `Proiezioni` JOIN `Film` ON `id` = `film`
                    WHERE `Proiezioni`.`cinema` = _cinema
						AND `Proiezioni`.`sala` = _sala
						AND `Proiezioni`.`data` = _data
						AND `Proiezioni`.`ora` = _ora);
	SELECT `matricola`, `Dipendenti`.`nome`, `cognome`
    FROM `Dipendenti` JOIN `Turni` ON `matricola` = `dipendente`
		JOIN `Proiezioni` ON `Proiezioni`.`cinema` = `Turni`.`cinema`
							AND GIORNO_DELLA_SETTIMANA(`Proiezioni`.`data`) = `Turni`.`giorno`
							AND `Proiezioni`.`cinema` = _cinema
                            AND `Proiezioni`.`sala` = _sala
                            AND `Proiezioni`.`data` = _data
                            AND `Proiezioni`.`ora` = _ora
		JOIN `Film` ON `id` = `Proiezioni`.`film`
    WHERE `ruolo` = 'Proiezionista' AND `inizio` <= _ora
		AND `Turni`.`durata` >= `Film`.`durata`
        AND NOT EXISTS (SELECT *
						FROM `Proiezioni` JOIN `Film` ON `id` = `film`
						WHERE `cinema` = _cinema
						AND `data` = _data
						AND `ora` <= _fine
						AND SEC_TO_TIME(TIME_TO_SEC(`ora`) + TIME_TO_SEC(`durata`))
							>= _ora
						AND `proiezionista` = `matricola`);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- function MESSAGGIO_ERRORE
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE FUNCTION `MESSAGGIO_ERRORE` (_codice INT)
RETURNS VARCHAR(128)
DETERMINISTIC
BEGIN
	RETURN (SELECT
			CASE
				WHEN _codice = 45001 THEN "L'orario di chiusura non può precedere quello di apertura"
				WHEN _codice = 45002 THEN "La sala selezionata è già impegnata in una proiezione nell'orario selezionato."
				WHEN _codice = 45003 THEN "Il cinema selezionato è chiuso nell'intervallo di tempo specificato."
				WHEN _codice = 45004 THEN "Impossibile assegnare ad una proiezione un dipendente non proiezionista."
				WHEN _codice = 45005 THEN "Il proiezionista selezionato non è assegnato ad un turno compatibile con la proiezione."
				WHEN _codice = 45006 THEN "Il proiezionista selezionato è già assegnato ad un'altra  proiezione nel periodo richiesto."
				WHEN _codice = 45007 THEN "Impossibile creare un turno di più di 8 ore."
				WHEN _codice = 45008 THEN "La somma della durata dei turni nella giornata supera le 8 ore."
				WHEN _codice = 45009 THEN "Il dipendente è già assegnato ad un turno nell'arco temporale selezionato."
				WHEN _codice = 45010 THEN "Impossibile creare una prenotazione non confermata."
				WHEN _codice = 45011 THEN "Impossibile cambiare lo stato di una prenotazione annullata, scaduta o validata."
				WHEN _codice = 45012 THEN "Non è possibile annullare una prenotazione raggiunti i trenta minuti precedenti l'inizio della proiezione."
				ELSE NULL
			END);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure mostra_dipendenti
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `mostra_dipendenti` ()
BEGIN
	SELECT * FROM `Dipendenti`;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserisci_dipendente
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `inserisci_dipendente` (
	IN _nome VARCHAR(45),
    IN _cognome VARCHAR(45),
    IN _ruolo VARCHAR(15))
BEGIN
	INSERT INTO `Dipendenti` (`nome`, `cognome`, `ruolo`)
    VALUES (_nome, _cognome, _ruolo);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure elimina_dipendente
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `elimina_dipendente` (IN _matricola INT)
BEGIN
	DELETE FROM `Dipendenti`
    WHERE `matricola` = _matricola;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserisci_cinema
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `inserisci_cinema` (
	IN _indirizzo VARCHAR(128),
    IN _apertura TIME,
    IN _chiusura TIME)
BEGIN
	INSERT INTO `Cinema` (`indirizzo`, `apertura`, `chiusura`)
    VALUES (_indirizzo, _apertura, _chiusura);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure elimina_cinema
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `elimina_cinema` (IN _id INT)
BEGIN
	DELETE FROM `Cinema`
    WHERE `id` = _id;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure mostra_sale
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `mostra_sale` (IN _cinema INT)
BEGIN
	SELECT `cinema`, `sala`,
		COUNT(DISTINCT fila) AS numero_file,
        COUNT(DISTINCT numero) AS posti_per_fila
    FROM `Posti`
    WHERE `cinema` = _cinema
    GROUP BY `cinema`, `sala`;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserisci_sala
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `inserisci_sala` (
	IN _cinema INT,
    IN _numero INT,
    IN _file INT,
    IN _posti_per_fila INT)
BEGIN
	INSERT INTO `Sale` (`cinema`, `numero`)
    VALUES (_cinema, _numero);
	INSERT INTO `Posti` (`cinema`, `sala`, `fila`, `numero`)
	WITH RECURSIVE numeri AS (SELECT 1 AS numero
								UNION ALL
								SELECT numero + 1
								FROM numeri
								WHERE numero + 1 <= _posti_per_fila),
					lettere AS (SELECT 'A' AS lettera
								UNION ALL
								SELECT CHAR(ASCII(lettera) + 1 USING ASCII)
								FROM lettere
								WHERE ASCII(lettera) + 1 <= ASCII('A') + _file)
	SELECT _cinema, _numero, lettera, numeri.numero
	FROM lettere CROSS JOIN numeri;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure elimina_sala
-- -----------------------------------------------------

DELIMITER $$
USE `cinemadb`$$
CREATE PROCEDURE `elimina_sala` (
	IN _cinema INT,
	IN _numero INT)
BEGIN
	DELETE FROM `Sale`
    WHERE `cinema` = _cinema AND `numero` = _numero;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- View `cinemadb`.`Palinsesti`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `cinemadb`.`Palinsesti`;
USE `cinemadb`;
CREATE  OR REPLACE VIEW `Palinsesti` AS
	SELECT `data`, `ora`, `cinema`, `sala`, `prezzo`,
		`id` AS movie_id, `nome`, `durata`,
		`casa_cinematografica`, `cast`
    FROM `Proiezioni` JOIN `Film` ON `film` = `id`
    WHERE `data` > CURDATE()
		OR (`data` = CURDATE() AND `ora` > TIME(NOW()))
	ORDER BY `data`, `ora`, `cinema`, `sala`;
USE `cinemadb`;

DELIMITER $$
USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Cinema_BEFORE_INSERT_Check_Orario`
BEFORE INSERT ON `Cinema`
FOR EACH ROW
BEGIN
	IF (NEW.`chiusura` < NEW.`apertura`) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45001);
		SIGNAL SQLSTATE '45001'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Cinema_BEFORE_UPDATE_Check_Orario`
BEFORE UPDATE ON `Cinema`
FOR EACH ROW
BEGIN
	IF (NEW.`chiusura` < NEW.`apertura`) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45001);
		SIGNAL SQLSTATE '45001'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Turni_BEFORE_INSERT_Check_Durata`
BEFORE INSERT ON `Turni`
FOR EACH ROW
BEGIN
    IF (NEW.`durata` > TIME('08:00:00')) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45007);
		SIGNAL SQLSTATE '45007'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Turni_BEFORE_INSERT_Check_Somma_Durate` 
BEFORE INSERT ON `Turni` 
FOR EACH ROW
BEGIN
	DECLARE OreTotali TIME;
    SET OreTotali = (SELECT SEC_TO_TIME(SUM(TIME_TO_SEC(`Turni`.`durata`)))
                        FROM `Turni`
                        WHERE `Turni`.`dipendente` = NEW.`dipendente`
                            AND `Turni`.`giorno` = NEW.`giorno`);
    IF (OreTotali + NEW.`durata` > TIME('08:00:00')) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45008);
		SIGNAL SQLSTATE '45008'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Turni_BEFORE_INSERT_Check_Sovrapposizioni`
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
		SET @err_msg = MESSAGGIO_ERRORE(45009);
		SIGNAL SQLSTATE '45009'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Turni_BEFORE_INSERT_Check_Cinema_Aperto`
BEFORE INSERT ON `Turni`
FOR EACH ROW
BEGIN
	DECLARE ora_apertura_cinema TIME;
    DECLARE ora_chiusura_cinema TIME;
    SET ora_apertura_cinema = (SELECT `apertura` FROM `Cinema` WHERE `id` = NEW.`cinema`);
    SET ora_chiusura_cinema = (SELECT `chiusura` FROM `Cinema` WHERE `id` = NEW.`cinema`);
    IF (NEW.`inizio` < ora_apertura_cinema
		OR SEC_TO_TIME(TIME_TO_SEC(NEW.`inizio`) + TIME_TO_SEC(NEW.`durata`)) > ora_chiusura_cinema) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45003);
		SIGNAL SQLSTATE '45003'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Turni_BEFORE_UPDATE_Check_Durata`
BEFORE UPDATE ON `Turni`
FOR EACH ROW
BEGIN
    IF (NEW.`durata` > TIME('08:00:00')) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45007);
		SIGNAL SQLSTATE '45007'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Turni_BEFORE_UPDATE_Check_Somma_Durate`
BEFORE UPDATE ON `Turni`
FOR EACH ROW
BEGIN
	DECLARE OreTotali TIME;
    SET OreTotali = (SELECT SEC_TO_TIME(SUM(TIME_TO_SEC(`Turni`.`durata`)))
                        FROM `Turni`
                        WHERE `Turni`.`dipendente` = NEW.`dipendente`
                            AND `Turni`.`giorno` = NEW.`giorno`);
    IF (OreTotali + NEW.`durata` > TIME('08:00:00')) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45008);
		SIGNAL SQLSTATE '45008'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Turni_BEFORE_UPDATE_Check_Sovrapposizioni`
BEFORE UPDATE ON `Turni`
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
		SET @err_msg = MESSAGGIO_ERRORE(45009);
		SIGNAL SQLSTATE '45009'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Turni_BEFORE_UPDATE_Check_Cinema_Aperto`
BEFORE UPDATE ON `Turni`
FOR EACH ROW
BEGIN
	DECLARE ora_apertura_cinema TIME;
    DECLARE ora_chiusura_cinema TIME;
    SET ora_apertura_cinema = (SELECT `apertura` FROM `Cinema` WHERE `id` = NEW.`cinema`);
    SET ora_chiusura_cinema = (SELECT `chiusura` FROM `Cinema` WHERE `id` = NEW.`cinema`);
    IF (NEW.`inizio` < ora_apertura_cinema
		OR SEC_TO_TIME(TIME_TO_SEC(NEW.`inizio`) + TIME_TO_SEC(NEW.`durata`)) > ora_chiusura_cinema) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45003);
		SIGNAL SQLSTATE '45003'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Turni_AFTER_DELETE_Reset_Proiezionista_Proiezione`
AFTER DELETE ON `Turni`
FOR EACH ROW
BEGIN
	UPDATE `Proiezioni`
    SET `proiezionista` = NULL
    WHERE (`cinema`, `sala`, `data`, `ora`) IN
		(SELECT `cinema`, `sala`, `data`, `ora`
			FROM `Proiezioni` JOIN `Film` ON `film` = `id`
			WHERE `proiezionista` = OLD.`dipendente`
				AND `cinema` = OLD.`cinema`
				AND GIORNO_DELLA_SETTIMANA(`data`) = OLD.`giorno`
				AND `ora` >= OLD.`inizio`
				AND SEC_TO_TIME(TIME_TO_SEC(`ora`) + TIME_TO_SEC(`Film`.`durata`))
					<= SEC_TO_TIME(TIME_TO_SEC(OLD.`inizio`) + TIME_TO_SEC(OLD.`durata`)));
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_INSERT_Check_Cinema_Aperto`
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
		SET @err_msg = MESSAGGIO_ERRORE(45003);
		SIGNAL SQLSTATE '45003'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_INSERT_Check_Sala_Libera`
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
		SET @err_msg = MESSAGGIO_ERRORE(45002);
		SIGNAL SQLSTATE '45002'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_INSERT_Check_Proiezionista`
BEFORE INSERT ON `Proiezioni`
FOR EACH ROW
BEGIN
	DECLARE _ruolo VARCHAR(15);
    SET _ruolo = (SELECT `ruolo` FROM `Dipendenti` WHERE `matricola` = NEW.`proiezionista`);
    IF (NEW.`proiezionista` IS NOT NULL AND _ruolo != 'Proiezionista') THEN
		SET @err_msg = MESSAGGIO_ERRORE(45004);
		SIGNAL SQLSTATE '45004'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_INSERT_Check_Proiezionista_Occupato`
BEFORE INSERT ON `Proiezioni`
FOR EACH ROW
BEGIN
	DECLARE _fine TIME;
	DECLARE impegnato BOOL;
    SET _fine = (SELECT SEC_TO_TIME(TIME_TO_SEC(NEW.`ora`) + TIME_TO_SEC(`durata`))
					FROM `Film`
                    WHERE `id` = NEW.`film`);
    SET impegnato = (SELECT COUNT(*)
						FROM `Proiezioni` JOIN `Film` ON `id` = `film`
						WHERE `cinema` = NEW.`cinema`
						AND `data` = NEW.`data`
						AND `proiezionista` = NEW.`proiezionista`
						AND `ora` <= _fine
						AND SEC_TO_TIME(TIME_TO_SEC(`ora`) + TIME_TO_SEC(`durata`))
							>= NEW.`ora`);
	IF (NEW.`proiezionista` IS NOT NULL AND impegnato = TRUE) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45006);
		SIGNAL SQLSTATE '45006'
		SET MESSAGE_TEXT = @err_msg;
	END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_INSERT_Check_Turno`
BEFORE INSERT ON `Proiezioni`
FOR EACH ROW
BEGIN
    DECLARE _durata TIME;
	DECLARE esiste_turno BOOL;
    SET _durata = (SELECT `durata` FROM `Film` WHERE NEW.`film` = `id`);
	SET esiste_turno = (SELECT COUNT(*)
						FROM `Turni`
						WHERE `Turni`.`cinema` = NEW.`cinema`
							AND `Turni`.`giorno` = GIORNO_DELLA_SETTIMANA(NEW.`data`)
							AND `Turni`.`dipendente` = NEW.`proiezionista`
							AND `inizio` <= NEW.`ora`
							AND SEC_TO_TIME(TIME_TO_SEC(NEW.`ora`) + TIME_TO_SEC(_durata))
							<= SEC_TO_TIME(TIME_TO_SEC(`inizio`) + TIME_TO_SEC(`durata`)));
	IF (NEW.`proiezionista` IS NOT NULL AND esiste_turno = FALSE) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45005);
		SIGNAL SQLSTATE '45005'
		SET MESSAGE_TEXT = @err_msg;
	END IF;
END$$

USE `cinemadb`$$
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
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_UPDATE_Check_Sala_Libera`
BEFORE UPDATE ON `Proiezioni`
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
											AND (`cinema`, `sala`, `data`, `ora`) NOT IN
												(SELECT OLD.`cinema`, OLD.`sala`, OLD.`data`, OLD.`ora`)
										ORDER BY `data` ASC, `ora` ASC
										LIMIT 1);
	SET fine_proiezione_prec = (SELECT TIMESTAMP(`data`, `ora` + `durata`)
										FROM `Proiezioni` JOIN `Film` on `id` = `film`
										WHERE `cinema` = NEW.`cinema` AND `sala` = NEW.`sala`
											AND (`data` < NEW.`data` OR (`data` = NEW.`data` AND `ora` <= NEW.`ora`))
											AND (`cinema`, `sala`, `data`, `ora`) NOT IN
												(SELECT OLD.`cinema`, OLD.`sala`, OLD.`data`, OLD.`ora`)
										ORDER BY `data` DESC, `ora` DESC
										LIMIT 1);
    IF ((fine_proiezione_prec IS NOT NULL AND fine_proiezione_prec > inizio_proiezione)
		OR (inizio_proiezione_succ IS NOT NULL AND inizio_proiezione_succ < fine_proiezione)) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45002);
		SIGNAL SQLSTATE '45002'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_UPDATE_Check_Proiezionista`
BEFORE UPDATE ON `Proiezioni`
FOR EACH ROW
BEGIN
	DECLARE _ruolo VARCHAR(15);
    SET _ruolo = (SELECT `ruolo` FROM `Dipendenti` WHERE `matricola` = NEW.`proiezionista`);
    IF (NEW.`proiezionista` IS NOT NULL AND _ruolo != 'Proiezionista') THEN
		SET @err_msg = MESSAGGIO_ERRORE(45004);
		SIGNAL SQLSTATE '45004'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_UPDATE_Check_Proiezionista_Occupato`
BEFORE UPDATE ON `Proiezioni`
FOR EACH ROW
BEGIN
	DECLARE _fine TIME;
	DECLARE impegnato BOOL;
    SET _fine = (SELECT SEC_TO_TIME(TIME_TO_SEC(NEW.`ora`) + TIME_TO_SEC(`durata`))
					FROM `Film`
                    WHERE `id` = NEW.`film`);
    SET impegnato = (SELECT COUNT(*)
						FROM `Proiezioni` JOIN `Film` ON `id` = `film`
						WHERE `cinema` = NEW.`cinema`
						AND `data` = NEW.`data`
						AND `proiezionista` = NEW.`proiezionista`
						AND `ora` <= _fine
						AND SEC_TO_TIME(TIME_TO_SEC(`ora`) + TIME_TO_SEC(`durata`))
							>= NEW.`ora`);
	IF (NEW.`proiezionista` IS NOT NULL AND impegnato = TRUE) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45006);
		SIGNAL SQLSTATE '45006'
		SET MESSAGE_TEXT = @err_msg;
	END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Proiezioni_BEFORE_UPDATE_Check_Turno`
BEFORE UPDATE ON `Proiezioni`
FOR EACH ROW
BEGIN
    DECLARE _durata TIME;
	DECLARE esiste_turno BOOL;
    SET _durata = (SELECT `durata` FROM `Film` WHERE NEW.`film` = `id`);
	SET esiste_turno = (SELECT COUNT(*)
						FROM `Turni`
						WHERE `Turni`.`cinema` = NEW.`cinema`
							AND `Turni`.`giorno` = GIORNO_DELLA_SETTIMANA(NEW.`data`)
							AND `Turni`.`dipendente` = NEW.`proiezionista`
							AND `inizio` <= NEW.`ora`
							AND SEC_TO_TIME(TIME_TO_SEC(NEW.`ora`) + TIME_TO_SEC(_durata))
							<= SEC_TO_TIME(TIME_TO_SEC(`inizio`) + TIME_TO_SEC(`durata`)));
	IF (NEW.`proiezionista` IS NOT NULL AND esiste_turno = FALSE) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45005);
		SIGNAL SQLSTATE '45005'
		SET MESSAGE_TEXT = @err_msg;
	END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Prenotazioni_BEFORE_INSERT_Check_Stato`
BEFORE INSERT ON `Prenotazioni`
FOR EACH ROW
BEGIN
	IF (NEW.`stato` != 'Confermata') THEN
		SET @err_msg = MESSAGGIO_ERRORE(45003);
		SIGNAL SQLSTATE '45010'
		SET MESSAGE_TEXT = @err_msg;
	END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Prenotazioni_BEFORE_UPDATE_Check_Stato`
BEFORE UPDATE ON `Prenotazioni`
FOR EACH ROW
BEGIN
	IF (OLD.`stato` != 'Confermata') THEN
		SET @err_msg = MESSAGGIO_ERRORE(45011);
		SIGNAL SQLSTATE '45011'
		SET MESSAGE_TEXT = @err_msg;
	END IF;
END$$

USE `cinemadb`$$
CREATE TRIGGER `cinemadb`.`Prenotazioni_BEFORE_UPDATE_Check_Ora_Proiezione`
BEFORE UPDATE ON `Prenotazioni`
FOR EACH ROW
BEGIN
	DECLARE inizio_proiezione TIMESTAMP;
    SET inizio_proiezione = (SELECT TIMESTAMP(`data`, `ora`)
								FROM `Prenotazioni`
                                WHERE `codice` = NEW.`codice`);
    IF (NEW.`stato` = 'Annullata'
			AND inizio_proiezione > DATE_SUB(NOW(), INTERVAL 30 MINUTE)) THEN
		SET @err_msg = MESSAGGIO_ERRORE(45012);
		SIGNAL SQLSTATE '45012'
		SET MESSAGE_TEXT = @err_msg;
    END IF;
END$$


DELIMITER ;
GRANT EXECUTE ON procedure `cinemadb`.`mostra_proiezioni` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`inserisci_proiezione` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`mostra_proiezionisti_disponibili` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`assegna_proiezionista` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`elimina_proiezione` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`mostra_turni` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`inserisci_turno` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`elimina_turno` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`mostra_stato_prenotazioni` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`mostra_proiezioni_senza_proiezionista` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`mostra_cinema_senza_maschere` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`mostra_dipendenti` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`inserisci_dipendente` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`elimina_dipendente` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`mostra_cinema` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`inserisci_cinema` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`elimina_cinema` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`mostra_sale` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`inserisci_sala` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`elimina_sala` TO 'amministratore';
GRANT EXECUTE ON procedure `cinemadb`.`mostra_cinema` TO 'cliente';
GRANT EXECUTE ON procedure `cinemadb`.`mostra_palinsesto` TO 'cliente';
GRANT EXECUTE ON procedure `cinemadb`.`mostra_posti_disponibili` TO 'cliente';
GRANT EXECUTE ON procedure `cinemadb`.`effettua_prenotazione` TO 'cliente';
GRANT EXECUTE ON procedure `cinemadb`.`annulla_prenotazione` TO 'cliente';
GRANT EXECUTE ON procedure `cinemadb`.`valida_prenotazione` TO 'maschera';

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
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (4, 'Via Cavour 1810 IM Imperia', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (5, 'Via Monte Bianco 94100 EN Enna', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (6, 'Via Giove 28100 NO Novara', '10:00:00', '23:00:00');
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
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Il padrino', '02:55:00', 'Paramount Pictures Studios, ALTRO', 'Marlon Brando, Al Pacino, James Caan, Richard S. Castellano, Robert Duvall, Diane Keaton, John Cazale, Talia Shire, Abe Vigoda, Al Lettieri, Gianni Russo, Lenny Montana');
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Il cavaliere oscuro', '02:32:00', 'Warner Bros. Pictures, ALTRO', 'Christian Bale, Cillian Murphy, Gary Oldman, Morgan Freeman, Heath Ledger, Michael Caine, Maggie Gyllenhaal, Aaron Eckhart');
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Frankenstein Junior', '01:46:00', 'Gruskoff/Venture Films, ALTRO', 'Gene Wilder, Peter Boyle, Marty Feldman, Teri Garr, Cloris Leachman');
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Pulp Fiction', '02:34:00', 'Miramax, ALTRO', NULL);
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Il buono, il brutto, il cattivo', '02:41:00', 'PEA', NULL);
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Fight Club', '02:19:00', 'Linson Films', NULL);
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Matrix', '02:16:00', 'Warner Bros., ALTRO', NULL);
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`) VALUES (DEFAULT, 'Amici miei', '02:20:00', 'Rizzoli Film, ALTRO', NULL);

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
    
DROP EVENT IF EXISTS `cinemadb`.`scadenza_prenotazioni`;
CREATE EVENT `cinemadb`.`scadenza_prenotazioni`
ON SCHEDULE EVERY 1 MINUTE
STARTS TIMESTAMP(CURDATE())
DO
    UPDATE `Prenotazioni` SET `stato`='Scaduta'
    WHERE `codice` IN (SELECT `codice`
						FROM `Prenotazioni` JOIN `Proiezioni` ON `Prenotazioni`.`cinema` = `Proiezioni`.`cinema`
																AND `Prenotazioni`.`sala` = `Proiezioni`.`sala`
																AND `Prenotazioni`.`data` = `Proiezioni`.`data`
																AND `Prenotazioni`.`ora` = `Proiezioni`.`ora`
											JOIN `Film` ON `film` = `id`
						WHERE `stato` = 'Confermata'
							AND CURRENT_TIMESTAMP() > TIMESTAMP(`Prenotazioni`.`data`,
								SEC_TO_TIME(TIME_TO_SEC(`Proiezioni`.`ora`) + TIME_TO_SEC(`durata`))));

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
-- Data for table `cinemadb`.`Proiezioni`
-- -----------------------------------------------------

INSERT INTO `Proiezioni` (`cinema`, `sala`, `data`, `ora`, `prezzo`, `film`)
WITH RECURSIVE giornate AS (SELECT CURDATE()
									+ INTERVAL 7 - WEEKDAY(CURDATE()) DAY
                                    AS giorno
							 UNION ALL
							 SELECT giorno + INTERVAL 1 DAY
							 FROM giornate
							 WHERE giorno + INTERVAL 1 DAY
								< CURDATE()
									+ INTERVAL 7 - WEEKDAY(CURDATE()) DAY
									+ INTERVAL 7 DAY)
SELECT cinema, numero, giorno, ora, '5.00',
	(SELECT id FROM Film ORDER BY RAND(0) LIMIT 1) AS id
FROM Sale CROSS JOIN giornate
		  CROSS JOIN (SELECT '10:00:00' AS ora
						UNION ALL SELECT '15:00:00'
                        UNION ALL SELECT '18:00:00'
                        UNION ALL SELECT '21:00:00') AS orari;

-- -----------------------------------------------------
-- Data for table `cinemadb`.`Turni`
-- -----------------------------------------------------

INSERT INTO `Turni` (`dipendente`, `giorno`, `inizio`, `durata`, `cinema`)
SELECT D.matricola, Giorni.nome AS giorno, Orari.inizio, Orari.durata, Cinema.id AS cinema
FROM (SELECT matricola FROM Dipendenti WHERE Dipendenti.ruolo = 'Maschera' ORDER BY matricola ASC LIMIT 20) AS D JOIN Giorni
				CROSS JOIN (SELECT '10:00:00' AS inizio, '05:00:00' AS durata) AS Orari
				JOIN Cinema ON MOD(matricola, (SELECT COUNT(*) FROM Cinema)) = Cinema.id
UNION ALL
SELECT D.matricola, Giorni.nome AS giorno, Orari.inizio, Orari.durata, Cinema.id AS cinema
FROM (SELECT matricola FROM Dipendenti WHERE Dipendenti.ruolo = 'Maschera' ORDER BY matricola DESC LIMIT 20) AS D JOIN Giorni
				CROSS JOIN (SELECT '15:00:00' AS inizio, '08:00:00' AS durata) AS Orari
				JOIN Cinema ON MOD(matricola, (SELECT COUNT(*) FROM Cinema)) = Cinema.id
UNION ALL
SELECT D.matricola, Giorni.nome AS giorno, Orari.inizio, Orari.durata, Cinema.id AS cinema
FROM (SELECT matricola FROM Dipendenti WHERE Dipendenti.ruolo = 'Proiezionista' ORDER BY matricola ASC LIMIT 50) AS D JOIN Giorni
				CROSS JOIN (SELECT '10:00:00' AS inizio, '05:00:00' AS durata) AS Orari
				JOIN Cinema ON MOD(matricola, (SELECT COUNT(*) FROM Cinema)) = Cinema.id
UNION ALL
SELECT D.matricola, Giorni.nome AS giorno, Orari.inizio, Orari.durata, Cinema.id AS cinema
FROM (SELECT matricola FROM Dipendenti WHERE Dipendenti.ruolo = 'Proiezionista' ORDER BY matricola DESC LIMIT 20) AS D JOIN Giorni
				CROSS JOIN (SELECT '15:00:00' AS inizio, '08:00:00' AS durata) AS Orari
				JOIN Cinema ON MOD(matricola, (SELECT COUNT(*) FROM Cinema)) = Cinema.id
ORDER BY matricola ASC, cinema ASC;

-- end attached script 'Popolazione'
