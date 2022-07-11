CREATE TABLE IF NOT EXISTS `cinemadb`.`Film`
(
    `id`                   INT           NOT NULL AUTO_INCREMENT,
    `nome`                 VARCHAR(63)   NOT NULL,
    `durata`               TIME          NOT NULL,
    `casa_cinematografica` VARCHAR(255)  NULL,
    `cast`                 VARCHAR(1023) NULL,
    PRIMARY KEY (`id`)
)
    ENGINE = InnoDB;
