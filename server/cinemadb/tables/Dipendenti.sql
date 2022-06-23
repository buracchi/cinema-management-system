CREATE TABLE IF NOT EXISTS `cinemadb`.`Dipendenti`
(
    `matricola` INT         NOT NULL AUTO_INCREMENT,
    `nome`      VARCHAR(63) NOT NULL,
    `cognome`   VARCHAR(63) NOT NULL,
    `ruolo`     VARCHAR(15) NOT NULL,
    PRIMARY KEY (`matricola`),
    INDEX `fk_Dipendenti_Ruoli1_idx` (`ruolo` ASC) VISIBLE,
    CONSTRAINT `fk_Dipendenti_Ruolo1`
        FOREIGN KEY (`ruolo`)
            REFERENCES `cinemadb`.`Ruoli` (`nome`)
            ON DELETE NO ACTION
            ON UPDATE NO ACTION
)
    ENGINE = InnoDB;
