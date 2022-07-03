START TRANSACTION;
USE `cinemadb`;
INSERT INTO `Sale` (`cinema`, `numero`)
WITH RECURSIVE numeri AS (SELECT 1 AS numero
                          UNION ALL
                          SELECT numero + 1
                          FROM numeri
                          WHERE numero + 1 <= 5)
SELECT id, numero
FROM cinema
         CROSS JOIN numeri;
COMMIT;
