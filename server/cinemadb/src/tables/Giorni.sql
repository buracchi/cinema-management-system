CREATE TABLE IF NOT EXISTS `cinemadb`.`Giorni`
(
    `nome`   VARCHAR(15)         NOT NULL,
    `numero` TINYINT(1) UNSIGNED NOT NULL,
    PRIMARY KEY (`nome`),
    UNIQUE INDEX `ordine_UNIQUE` (`numero` ASC) VISIBLE
)
    ENGINE = InnoDB;
