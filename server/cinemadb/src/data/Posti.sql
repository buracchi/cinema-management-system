START TRANSACTION;
USE `cinemadb`;
INSERT INTO `Posti` (`cinema`, `sala`, `fila`, `numero`)
WITH RECURSIVE
    numeri AS (SELECT 1 AS numero
               UNION ALL
               SELECT numero + 1
               FROM numeri
               WHERE numero + 1 <= 25),
    lettere AS (SELECT 'A' AS lettera
                UNION ALL
                SELECT CHAR(ASCII(lettera) + 1 USING ASCII)
                FROM lettere
                WHERE ASCII(lettera) + 1 <= ASCII('A') + 9)
SELECT cinema, Sale.numero, lettera, numeri.numero
FROM `Sale`
         CROSS JOIN lettere
         CROSS JOIN numeri;
COMMIT;
