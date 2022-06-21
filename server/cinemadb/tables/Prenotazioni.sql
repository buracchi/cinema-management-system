CREATE TABLE IF NOT EXISTS `cinemadb`.`Prenotazioni` (
  `codice` INT NOT NULL,
  `transazione` VARCHAR(256) NULL,
  `cinema` INT NOT NULL,
  `sala` INT NOT NULL,
  `fila` CHAR(1) NOT NULL,
  `numero` INT NOT NULL,
  `data` DATE NOT NULL,
  `ora` TIME NOT NULL,
  `stato` VARCHAR(15) NOT NULL,
  `timestamp` TIMESTAMP NOT NULL,
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
