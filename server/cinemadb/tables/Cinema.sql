CREATE TABLE IF NOT EXISTS `cinemadb`.`Cinema`
(
    `id`        INT          NOT NULL AUTO_INCREMENT,
    `indirizzo` VARCHAR(128) NOT NULL,
    `apertura`  TIME         NOT NULL,
    `chiusura`  TIME         NOT NULL,
    PRIMARY KEY (`id`),
    UNIQUE INDEX `indirizzo_UNIQUE` (`indirizzo` ASC) VISIBLE
)
    ENGINE = InnoDB;
