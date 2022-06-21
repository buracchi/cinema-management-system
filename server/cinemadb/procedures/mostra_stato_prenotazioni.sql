CREATE PROCEDURE `mostra_stato_prenotazioni` ()
BEGIN
	SELECT `Sale`.`cinema`, `indirizzo`, `Sale`.`numero` AS sala,
		IFNULL(SUM(`stato` = 'Confermata'
			OR `stato` = 'Validata'
            OR `stato` = 'Scaduta'), 0) AS confermate,
		IFNULL(SUM(`stato` = 'Annullata'), 0) AS annullate,
		IFNULL(SUM(`stato` = 'Validata'), 0) AS validate,
		IFNULL(SUM(`stato` = 'Scaduta'), 0) AS scadute
	FROM `Prenotazioni`
		RIGHT OUTER JOIN `Sale`
			ON `Prenotazioni`.`cinema` = `Sale`.`cinema`
				AND `sala` = `Sale`.`numero`
		JOIN `Cinema`
			ON `Sale`.`cinema` = `Cinema`.`id`
    GROUP BY `Sale`.`cinema`, `indirizzo`, `Sale`.`numero`
    ORDER BY `Sale`.`cinema`, `Sale`.`numero`;
END
