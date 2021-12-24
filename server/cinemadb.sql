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
-- Table `cinemadb`.`cinema`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `cinemadb`.`cinema` ;

CREATE TABLE IF NOT EXISTS `cinemadb`.`cinema` (
  `id` INT NOT NULL,
  `indirizzo` VARCHAR(45) NOT NULL,
  `orario` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`dipendente`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `cinemadb`.`dipendente` ;

CREATE TABLE IF NOT EXISTS `cinemadb`.`dipendente` (
  `id` INT NOT NULL,
  `nome` VARCHAR(45) NOT NULL,
  `cognome` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`turno`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `cinemadb`.`turno` ;

CREATE TABLE IF NOT EXISTS `cinemadb`.`turno` (
  `dipendente_id` INT NOT NULL,
  `giorno` VARCHAR(45) NOT NULL,
  `orario` VARCHAR(45) NOT NULL,
  `cinema_id` INT NOT NULL,
  PRIMARY KEY (`dipendente_id`, `orario`, `giorno`),
  INDEX `fk_turno_cinema1_idx` (`cinema_id` ASC) VISIBLE,
  INDEX `fk_turno_dipendente1_idx` (`dipendente_id` ASC) VISIBLE,
  CONSTRAINT `fk_turno_cinema1`
    FOREIGN KEY (`cinema_id`)
    REFERENCES `cinemadb`.`cinema` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_turno_dipendente1`
    FOREIGN KEY (`dipendente_id`)
    REFERENCES `cinemadb`.`dipendente` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`Sala`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `cinemadb`.`Sala` ;

CREATE TABLE IF NOT EXISTS `cinemadb`.`Sala` (
  `cinema_id` INT NOT NULL,
  `numero` INT NOT NULL,
  PRIMARY KEY (`cinema_id`, `numero`),
  INDEX `fk_Sala_cinema_idx` (`cinema_id` ASC) VISIBLE,
  CONSTRAINT `fk_Sala_cinema`
    FOREIGN KEY (`cinema_id`)
    REFERENCES `cinemadb`.`cinema` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`Posto`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `cinemadb`.`Posto` ;

CREATE TABLE IF NOT EXISTS `cinemadb`.`Posto` (
  `cinema_id` INT NOT NULL,
  `numero_sala` INT NOT NULL,
  `fila` VARCHAR(1) NOT NULL,
  `numero` INT NOT NULL,
  PRIMARY KEY (`cinema_id`, `numero_sala`, `numero`, `fila`),
  CONSTRAINT `fk_Posto_Sala1`
    FOREIGN KEY (`numero_sala` , `cinema_id`)
    REFERENCES `cinemadb`.`Sala` (`numero` , `cinema_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`film`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `cinemadb`.`film` ;

CREATE TABLE IF NOT EXISTS `cinemadb`.`film` (
  `id` INT NOT NULL,
  `nome` VARCHAR(45) NOT NULL,
  `durata` VARCHAR(45) NOT NULL,
  `casa_cinematografica` VARCHAR(45) NOT NULL,
  `cast` VARCHAR(255) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`Proiezione`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `cinemadb`.`Proiezione` ;

CREATE TABLE IF NOT EXISTS `cinemadb`.`Proiezione` (
  `cinema_id` INT NOT NULL,
  `numero_sala` INT NOT NULL,
  `data` VARCHAR(45) NOT NULL,
  `ora` VARCHAR(45) NOT NULL,
  `prezzo` VARCHAR(45) NOT NULL,
  `film_id` INT NOT NULL,
  `proiezionista_id` INT NULL,
  PRIMARY KEY (`cinema_id`, `numero_sala`, `data`, `ora`),
  INDEX `fk_Proiezione_Sala1_idx` (`cinema_id` ASC, `numero_sala` ASC) VISIBLE,
  INDEX `fk_Proiezione_Film1_idx` (`film_id` ASC) VISIBLE,
  INDEX `fk_Proiezione_dipendente1_idx` (`proiezionista_id` ASC) VISIBLE,
  CONSTRAINT `fk_Proiezione_Sala1`
    FOREIGN KEY (`cinema_id` , `numero_sala`)
    REFERENCES `cinemadb`.`Sala` (`cinema_id` , `numero`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Proiezione_Film1`
    FOREIGN KEY (`film_id`)
    REFERENCES `cinemadb`.`film` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Proiezione_dipendente1`
    FOREIGN KEY (`proiezionista_id`)
    REFERENCES `cinemadb`.`dipendente` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`Prenotazione`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `cinemadb`.`Prenotazione` ;

CREATE TABLE IF NOT EXISTS `cinemadb`.`Prenotazione` (
  `codice` INT UNSIGNED NOT NULL,
  `codice_transazione` VARCHAR(45) NOT NULL,
  `stato` VARCHAR(45) NOT NULL,
  `posto_cinema_id` INT NOT NULL,
  `posto_numero_sala` INT NOT NULL,
  `posto_numero` INT NOT NULL,
  `posto_fila` VARCHAR(1) NOT NULL,
  `proiezione_cinema_id` INT NOT NULL,
  `proiezione_numero_sala` INT NOT NULL,
  `proiezione_data` VARCHAR(45) NOT NULL,
  `proiezione_ora` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`codice`),
  INDEX `fk_Prenotazione_Posto1_idx` (`posto_cinema_id` ASC, `posto_numero_sala` ASC, `posto_numero` ASC, `posto_fila` ASC) VISIBLE,
  INDEX `fk_Prenotazione_Proiezione1_idx` (`proiezione_cinema_id` ASC, `proiezione_numero_sala` ASC, `proiezione_data` ASC, `proiezione_ora` ASC) VISIBLE,
  CONSTRAINT `fk_Prenotazione_Posto1`
    FOREIGN KEY (`posto_cinema_id` , `posto_numero_sala` , `posto_numero` , `posto_fila`)
    REFERENCES `cinemadb`.`Posto` (`cinema_id` , `numero_sala` , `numero` , `fila`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Prenotazione_Proiezione1`
    FOREIGN KEY (`proiezione_cinema_id` , `proiezione_numero_sala` , `proiezione_data` , `proiezione_ora`)
    REFERENCES `cinemadb`.`Proiezione` (`cinema_id` , `numero_sala` , `data` , `ora`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `cinemadb`.`ruolo_dipendente`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `cinemadb`.`ruolo_dipendente` ;

CREATE TABLE IF NOT EXISTS `cinemadb`.`ruolo_dipendente` (
  `nome` VARCHAR(45) NOT NULL,
  `dipendente_id` INT NOT NULL,
  PRIMARY KEY (`nome`, `dipendente_id`),
  INDEX `fk_ruolo_dipendente_dipendente1_idx` (`dipendente_id` ASC) VISIBLE,
  CONSTRAINT `fk_ruolo_dipendente_dipendente1`
    FOREIGN KEY (`dipendente_id`)
    REFERENCES `cinemadb`.`dipendente` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
