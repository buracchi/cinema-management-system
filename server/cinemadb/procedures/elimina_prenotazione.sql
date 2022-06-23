CREATE PROCEDURE `elimina_prenotazione`(IN _codice CHAR(6))
BEGIN
    DELETE
    FROM `Prenotazioni`
    WHERE `codice` = _codice
      AND `stato` = 'Attesa';
END
