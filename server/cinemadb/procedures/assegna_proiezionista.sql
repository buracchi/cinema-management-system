CREATE PROCEDURE `assegna_proiezionista` (
	IN _proiezionista INT,
    IN _cinema INT,
	IN _sala INT,
	IN _data DATE,
	IN _ora TIME)
BEGIN
	UPDATE `Proiezioni`
    SET `proiezionista` = _proiezionista
    WHERE `cinema` = _cinema AND `sala` = _sala
		AND `data` = _data AND `ora` = _ora;
END
