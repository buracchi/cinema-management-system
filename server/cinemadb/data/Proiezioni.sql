START TRANSACTION;
INSERT INTO `Proiezioni` (`cinema`, `sala`, `data`, `ora`, `prezzo`, `film`)
WITH RECURSIVE giornate AS (SELECT CURDATE()
                                       + INTERVAL 7 - WEEKDAY(CURDATE()) DAY
                                       AS giorno
                            UNION ALL
                            SELECT giorno + INTERVAL 1 DAY
                            FROM giornate
                            WHERE giorno + INTERVAL 1 DAY
                                      < CURDATE()
                                      + INTERVAL 7 - WEEKDAY(CURDATE()) DAY
                                      + INTERVAL 7 DAY)
SELECT cinema,
       numero,
       giorno,
       ora,
       '5.00',
       (SELECT id FROM Film ORDER BY RAND(0) LIMIT 1) AS id
FROM Sale
         CROSS JOIN giornate
         CROSS JOIN (SELECT '10:00:00' AS ora
                     UNION ALL
                     SELECT '15:00:00'
                     UNION ALL
                     SELECT '18:00:00'
                     UNION ALL
                     SELECT '21:00:00') AS orari;
COMMIT;
