CREATE TABLE IF NOT EXISTS `cinemadb`.`Cinema`
(
    `id`        INT          NOT NULL AUTO_INCREMENT,
    `indirizzo` VARCHAR(127) NOT NULL,
    `apertura`  TIME         NOT NULL,
    `chiusura`  TIME         NOT NULL,
    PRIMARY KEY (`id`),
    UNIQUE INDEX `indirizzo_UNIQUE` (`indirizzo` ASC) VISIBLE,
    CONSTRAINT `chk_Cinema_Apertura_Precede_Chiusura` CHECK (`apertura` < `chiusura`)
)
    ENGINE = InnoDB;
