CREATE PROCEDURE `elimina_prenotazione`(IN _codice_prenotazione VARCHAR(6))
BEGIN
    DELETE
    FROM `Prenotazioni`
    WHERE `codice` = CONV(_codice_prenotazione, 16, 10)
      AND `stato` = 'Attesa';
END
