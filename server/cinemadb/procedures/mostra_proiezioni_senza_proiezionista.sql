CREATE PROCEDURE `mostra_proiezioni_senza_proiezionista`()
BEGIN
    SELECT `cinema`, `sala`, `data`, `ora`, `prezzo`, `nome` AS film
    FROM `Proiezioni`
             JOIN `Film` ON `Proiezioni`.`film` = `Film`.`id`
    WHERE `proiezionista` IS NULL
        AND `data` > CURDATE()
       OR (`data` = CURDATE() AND `ora` > TIME(NOW()));
END
