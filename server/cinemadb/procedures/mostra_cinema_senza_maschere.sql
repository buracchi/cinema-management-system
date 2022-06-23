CREATE PROCEDURE `mostra_cinema_senza_maschere`()
BEGIN
    WITH calendario_cinema AS (SELECT `id`   AS cinema,
                                      `indirizzo`,
                                      `nome` AS giorno,
                                      `apertura`,
                                      `chiusura`
                               FROM `Cinema`
                                        CROSS JOIN `Giorni`),
         eventi AS (SELECT `cinema`, `indirizzo`, `giorno`, `apertura` AS `ora`, 0 AS `variazione`
                    FROM calendario_cinema
                    UNION ALL
                    SELECT `cinema`, `indirizzo`, `giorno`, `inizio`, 1
                    FROM `Turni`
                             JOIN `Dipendenti` ON `dipendente` = `matricola`
                             JOIN `Cinema` ON `cinema` = `id`
                    WHERE `ruolo` = 'Maschera'
                    UNION ALL
                    SELECT `cinema`,
                           `indirizzo`,
                           `giorno`,
                           SEC_TO_TIME(TIME_TO_SEC(`inizio`) + TIME_TO_SEC(`durata`)),
                           -1
                    FROM `Turni`
                             JOIN `Dipendenti` ON `dipendente` = `matricola`
                             JOIN `Cinema` ON `cinema` = `id`
                    WHERE `ruolo` = 'Maschera'
                    UNION ALL
                    SELECT `cinema`, `indirizzo`, `giorno`, `chiusura`, 0
                    FROM calendario_cinema),
         invervalli_eventi AS (SELECT `cinema`,
                                      `indirizzo`,
                                      `giorno`,
                                      `ora`               AS `dalle_ore`,
                                      LEAD(`ora`) OVER (PARTITION BY `cinema`, `giorno`
                                          ORDER BY `ora`) AS `alle_ore`,
                                      SUM(`variazione`) OVER (PARTITION BY `cinema`, `giorno`
                                          ORDER BY `ora`) AS numero_maschere
                               FROM eventi),
         gruppi_variazioni AS (SELECT `cinema`,
                                      `indirizzo`,
                                      `giorno`,
                                      `dalle_ore`,
                                      `alle_ore`,
                                      `numero_maschere`,
                                      IF(`numero_maschere`
                                             != lag(`numero_maschere`)
                                                    OVER (PARTITION BY `cinema`, `giorno`
                                                        ORDER BY `dalle_ore`, `alle_ore`),
                                         1, 0) AS variazione
                               FROM invervalli_eventi),
         gruppi_orari AS (SELECT `cinema`,
                                 `indirizzo`,
                                 `giorno`,
                                 `dalle_ore`,
                                 `alle_ore`,
                                 `numero_maschere`,
                                 sum(`variazione`) OVER (PARTITION BY `cinema`, `giorno`
                                     ORDER BY `dalle_ore`, `alle_ore`)
                                     AS `gruppo`
                          FROM gruppi_variazioni)
    SELECT `cinema`,
           `indirizzo`,
           `giorno`,
           MIN(`dalle_ore`) AS `dalle_ore`,
           MAX(`alle_ore`)  AS `alle_ore`,
           `numero_maschere`
    FROM gruppi_orari
    WHERE `dalle_ore` != `alle_ore`
    GROUP BY `cinema`, `indirizzo`, `giorno`, `numero_maschere`, `gruppo`
    HAVING `numero_maschere` < 2
    ORDER BY `cinema`, NUMERO_GIORNO(`giorno`), MIN(`dalle_ore`);
END
