CREATE PROCEDURE `inserisci_sala`(
    IN _cinema INT,
    IN _numero INT,
    IN _file INT,
    IN _posti_per_fila INT)
BEGIN
    DECLARE _troppe_file CONDITION FOR SQLSTATE '45024';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45024);
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;
    -- Qualunque livello di isolamento effettua la medesima strategia di
    -- locking per questa transazione.
    -- Non e' pertanto necessario specificare un livello di isolamento diverso
    -- da quello di default.
    IF (_file > 26) THEN
        SIGNAL _troppe_file SET MESSAGE_TEXT = _err_msg;
    END IF;
    START TRANSACTION;
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
                    WHERE ASCII(lettera) + 2 <= ASCII('A') + _file)
    SELECT _cinema, _numero, lettera, numeri.numero
    FROM lettere
             CROSS JOIN numeri;
    COMMIT;
END
