CREATE PROCEDURE `inserisci_turno` (
	IN _dipendente INT,
	IN _giorno VARCHAR(15),
	IN _inizio TIME,
	IN _durata TIME,
	IN _cinema INT)
BEGIN
	INSERT INTO `Turni` (`dipendente`, `giorno`, `inizio`, `durata`, `cinema`)
    VALUES (_dipendente, _giorno, _inizio, _durata, _cinema);
END
