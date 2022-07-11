CREATE PROCEDURE `elimina_proiezione`(
    IN _cinema INT,
    IN _sala INT,
    IN _data DATE,
    IN _ora TIME)
BEGIN
    DECLARE _proiezione_invalida CONDITION FOR SQLSTATE '45017';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45017);
    DECLARE _affected_rows INT;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;
    -- Con il livello di isolamento REPEATABLE READ, per operazioni di DELETE
    -- che coinvolgono un indice univoco con una condizione di ricerca univoca,
    -- InnoDB effettua il lock solo del record trovato e non un gap lock.
    -- Questa strategia di locking e' equivalente a quella adotta nell'analogo
    -- caso utilizzando il livello di isolamento READ COMMITTED.
    -- Non e' pertanto necessario specificare un livello di isolamento diverso
    -- da quello di default.
    START TRANSACTION;
    DELETE
    FROM `Proiezioni`
    WHERE `cinema` = _cinema
      AND `sala` = _sala
      AND `data` = _data
      AND `ora` = _ora;
    GET DIAGNOSTICS _affected_rows = ROW_COUNT;
    IF (_affected_rows = 0) THEN
        SIGNAL _proiezione_invalida SET MESSAGE_TEXT = _err_msg;
    END IF;
    COMMIT;
END
