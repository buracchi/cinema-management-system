DROP EVENT IF EXISTS `cinemadb`.`pulizia_proiezioni`;
CREATE EVENT `cinemadb`.`pulizia_proiezioni`
    ON SCHEDULE EVERY 1 DAY
        STARTS TIMESTAMP(CURDATE())
    DO
    DELETE
    FROM `Proiezioni`
    WHERE `data` < CURDATE();
