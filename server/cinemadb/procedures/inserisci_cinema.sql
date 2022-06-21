CREATE PROCEDURE `inserisci_cinema` (
	IN _indirizzo VARCHAR(128),
    IN _apertura TIME,
    IN _chiusura TIME)
BEGIN
	INSERT INTO `Cinema` (`indirizzo`, `apertura`, `chiusura`)
    VALUES (_indirizzo, _apertura, _chiusura);
END
