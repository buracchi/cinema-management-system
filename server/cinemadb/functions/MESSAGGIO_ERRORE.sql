CREATE FUNCTION `MESSAGGIO_ERRORE`(_codice INT)
    RETURNS VARCHAR(128)
    DETERMINISTIC
BEGIN
    RETURN (SELECT CASE
                       WHEN _codice = 45001
                           THEN "L'orario di chiusura non puo' precedere quello di apertura"
                       WHEN _codice = 45002
                           THEN "La sala e' impegnata in una proiezione nell'orario selezionato."
                       WHEN _codice = 45003
                           THEN "Il cinema selezionato e' chiuso nell'intervallo di tempo specificato."
                       WHEN _codice = 45004
                           THEN "E' possibile assegnare ad una proiezione solamente proiezionisti."
                       WHEN _codice = 45005
                           THEN "La proiezione non e' compatibile con i turni del proiezionista."
                       WHEN _codice = 45006
                           THEN "Il proiezionista e' gia' assegnato ad una proiezione nel periodo selezionato."
                       WHEN _codice = 45007
                           THEN "Impossibile creare un turno di piu' di 8 ore."
                       WHEN _codice = 45008
                           THEN "La somma della durata dei turni nella giornata supera le 8 ore."
                       WHEN _codice = 45009
                           THEN "Il dipendente e' gia' assegnato ad un turno nel periodo selezionato."
                       WHEN _codice = 45010
                           THEN "Impossibile creare una prenotazione non in attesa."
                       WHEN _codice = 45011
                           THEN "Impossibile modificare prenotazioni annullate, scadute o validate."
                       WHEN _codice = 45012
                           THEN "Non e' possibile annullare una prenotazione superati i trenta minuti precedenti l'inizio della proiezione."
                       WHEN _codice = 45013
                           THEN "Codice prenotazione non valido."
                       WHEN _codice = 45014
                           THEN "Il cinema selezionato e' invalido o inesistente."
                       WHEN _codice = 45015
                           THEN "Il posto selezionato non e' disponibile."
                       WHEN _codice = 45016
                           THEN "Il dipendente selezionato e' invalido o inesistente."
                       WHEN _codice = 45017
                           THEN "La proiezione selezionata e' invalida o inesistente."
                       WHEN _codice = 45018
                           THEN "La sala selezionata e' invalida o inesistente."
                       WHEN _codice = 45019
                           THEN "Il turno selezionato e' invalido o inesistente."
                       WHEN _codice = 45020
                           THEN "Impossibile confermare una prenotazione non in attesa."
                       WHEN _codice = 45021
                           THEN "Una prenotazione in attesa puo' essere solamente confermata."
                       WHEN _codice = 45022
                           THEN "Impossibile completare l'operazione, riprovare in un secondo momento."
                       WHEN _codice = 45023
                           THEN "Impossibile modificare il codice di prenotazione."
                       END);
END
