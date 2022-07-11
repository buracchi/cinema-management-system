CREATE TABLE IF NOT EXISTS `cinemadb`.`Sale`
(
    `cinema` INT NOT NULL,
    `numero` INT NOT NULL,
    PRIMARY KEY (`cinema`, `numero`),
    INDEX `fk_Sale_Cinema_idx` (`cinema` ASC) VISIBLE,
    CONSTRAINT `fk_Sale_Cinema`
        FOREIGN KEY (`cinema`)
            REFERENCES `cinemadb`.`Cinema` (`id`)
            ON DELETE CASCADE
            ON UPDATE CASCADE
)
    ENGINE = InnoDB;
