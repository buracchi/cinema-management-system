CREATE PROCEDURE `elimina_dipendente`(IN _matricola INT)
BEGIN
    DECLARE _dipendente_invalido CONDITION FOR SQLSTATE '45016';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45016);
    DECLARE _affected_rows INT;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;
    -- Con il livello di isolamento REPEATABLE READ, per operazioni di DELETE
    -- che coinvolgono un indice univoco con una condizione di ricerca univoca,
    -- InnoDB effettua il lock solo del record trovato e non sul gap che lo precede.
    -- Questa strategia di locking e' equivalente a quella adotta nell'analogo
    -- caso utilizzando il livello di isolamento READ COMMITTED.
    -- Non e' pertanto necessario specificare un livello di isolamento diverso
    -- da quello di default.
    START TRANSACTION;
    DELETE
    FROM `Dipendenti`
    WHERE `matricola` = _matricola;
    GET DIAGNOSTICS _affected_rows = ROW_COUNT;
    IF (_affected_rows = 0) THEN
        SIGNAL _dipendente_invalido SET MESSAGE_TEXT = _err_msg;
    END IF;
    COMMIT;
END
