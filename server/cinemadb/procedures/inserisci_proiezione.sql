CREATE PROCEDURE `inserisci_proiezione` (
	IN _cinema INT,
	IN _sala INT,
	IN _data DATE,
	IN _ora TIME,
	IN _prezzo NUMERIC(15,2),
	IN _film INT)
BEGIN
	INSERT INTO `Proiezioni` (`cinema`, `sala`, `data`, `ora`, `prezzo`, `film`)
    VALUES (_cinema, _sala, _data, _ora, _prezzo, _film);
END
