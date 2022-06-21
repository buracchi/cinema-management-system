CREATE PROCEDURE `inserisci_dipendente` (
	IN _nome VARCHAR(45),
    IN _cognome VARCHAR(45),
    IN _ruolo VARCHAR(15))
BEGIN
	INSERT INTO `Dipendenti` (`nome`, `cognome`, `ruolo`)
    VALUES (_nome, _cognome, _ruolo);
END
