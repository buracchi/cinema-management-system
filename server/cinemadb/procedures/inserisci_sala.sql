CREATE PROCEDURE `inserisci_sala`(
    IN _cinema INT,
    IN _numero INT,
    IN _file INT,
    IN _posti_per_fila INT)
BEGIN
    INSERT INTO `Sale` (`cinema`, `numero`)
    VALUES (_cinema, _numero);
    INSERT INTO `Posti` (`cinema`, `sala`, `fila`, `numero`)
    WITH RECURSIVE
        numeri AS (SELECT 1 AS numero
                   UNION ALL
                   SELECT numero + 1
                   FROM numeri
                   WHERE numero + 1 <= _posti_per_fila),
        lettere AS (SELECT 'A' AS lettera
                    UNION ALL
                    SELECT CHAR(ASCII(lettera) + 1 USING ASCII)
                    FROM lettere
                    WHERE ASCII(lettera) + 1 <= ASCII('A') + _file)
    SELECT _cinema, _numero, lettera, numeri.numero
    FROM lettere
             CROSS JOIN numeri;
END
