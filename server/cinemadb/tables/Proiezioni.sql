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
  INDEX `order_idx` USING BTREE (`data`, `ora`, `cinema`, `sala`) VISIBLE,
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
