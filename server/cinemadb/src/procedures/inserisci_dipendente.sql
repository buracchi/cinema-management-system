CREATE PROCEDURE `inserisci_dipendente`(
    IN _nome VARCHAR(63),
    IN _cognome VARCHAR(63),
    IN _ruolo VARCHAR(15))
BEGIN
    INSERT INTO `Dipendenti` (`nome`, `cognome`, `ruolo`)
    VALUES (_nome, _cognome, _ruolo);
END
