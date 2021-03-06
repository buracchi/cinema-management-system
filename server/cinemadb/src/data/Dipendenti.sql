START TRANSACTION;
USE `cinemadb`;
INSERT INTO `Dipendenti` (`matricola`, `nome`, `cognome`, `ruolo`)
WITH RECURSIVE matricole AS (SELECT 1 AS matricola
                             UNION ALL
                             SELECT matricola + 1
                             FROM matricole
                             WHERE matricola + 1 <= 200)
SELECT matricola,
       'Mario',
       'Rossi',
       IF(matricola <= 50, 'Maschera', 'Proiezionista')
FROM matricole;
COMMIT;
