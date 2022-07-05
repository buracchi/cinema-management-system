CREATE TABLE IF NOT EXISTS `cinemadb`.`Posti`
(
    `cinema` INT     NOT NULL,
    `sala`   INT     NOT NULL,
    `fila`   CHAR(1) NOT NULL,
    `numero` INT     NOT NULL,
    PRIMARY KEY (`cinema`, `sala`, `fila`, `numero`),
    CONSTRAINT `fk_Posti_Sale1`
        FOREIGN KEY (`cinema`, `sala`)
            REFERENCES `cinemadb`.`Sale` (`cinema`, `numero`)
            ON DELETE CASCADE
            ON UPDATE CASCADE,
    CONSTRAINT `chk_Posti_Fila_Alfabetica` CHECK (`fila` RLIKE '[A-Z]')
)
    ENGINE = InnoDB;
