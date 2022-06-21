CREATE PROCEDURE `mostra_proiezioni` ()
BEGIN
	SELECT `data`, `cinema`, `indirizzo`, `sala`,
		`film`, `Film`.`nome`, `prezzo`, `ora`, `durata`,
        `proiezionista`, `Dipendenti`.`nome`, `cognome`
    FROM `Proiezioni` JOIN `Film` ON `film` = `id`
		JOIN `Cinema` ON `cinema` = `Cinema`.`id`
        LEFT JOIN `Dipendenti` ON `matricola` = `proiezionista`
    WHERE `data` > CURDATE()
		OR (`data` = CURDATE() AND `ora` > TIME(NOW()))
	ORDER BY `data`, `ora`, `cinema`, `sala`;
END
