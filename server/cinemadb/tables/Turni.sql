CREATE TABLE IF NOT EXISTS `cinemadb`.`Turni`
(
    `dipendente` INT         NOT NULL,
    `giorno`     VARCHAR(15) NOT NULL,
    `inizio`     TIME        NOT NULL,
    `durata`     TIME        NOT NULL,
    `cinema`     INT         NOT NULL,
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
            ON UPDATE NO ACTION
)
    ENGINE = InnoDB;
