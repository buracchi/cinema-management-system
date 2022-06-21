CREATE TABLE IF NOT EXISTS `cinemadb`.`Film`
(
    `id`                   INT           NOT NULL AUTO_INCREMENT,
    `nome`                 VARCHAR(45)   NOT NULL,
    `durata`               TIME          NOT NULL,
    `casa_cinematografica` VARCHAR(256)  NULL,
    `cast`                 VARCHAR(1024) NULL,
    PRIMARY KEY (`id`)
)
    ENGINE = InnoDB;
