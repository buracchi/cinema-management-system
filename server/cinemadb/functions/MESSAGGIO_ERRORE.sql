CREATE FUNCTION `MESSAGGIO_ERRORE`(_codice INT)
    RETURNS VARCHAR(128)
    DETERMINISTIC
BEGIN
    RETURN (SELECT CASE
                       WHEN _codice = 45001 THEN "L'orario di chiusura non puo' precedere quello di apertura"
                       WHEN _codice = 45002
                           THEN "La sala selezionata e' gia' impegnata in una proiezione nell'orario selezionato."
                       WHEN _codice = 45003 THEN "Il cinema selezionato e' chiuso nell'intervallo di tempo specificato."
                       WHEN _codice = 45004
                           THEN "Impossibile assegnare ad una proiezione un dipendente non proiezionista."
                       WHEN _codice = 45005
                           THEN "Il proiezionista selezionato non e' assegnato ad un turno compatibile con la proiezione."
                       WHEN _codice = 45006
                           THEN "Il proiezionista selezionato e' già assegnato ad un'altra  proiezione nel periodo richiesto."
                       WHEN _codice = 45007 THEN "Impossibile creare un turno di piu' di 8 ore."
                       WHEN _codice = 45008 THEN "La somma della durata dei turni nella giornata supera le 8 ore."
                       WHEN _codice = 45009
                           THEN "Il dipendente e' gia' assegnato ad un turno nell'arco temporale selezionato."
                       WHEN _codice = 45010 THEN "Impossibile creare una prenotazione non in attesa."
                       WHEN _codice = 45011
                           THEN "Impossibile cambiare lo stato di una prenotazione annullata, scaduta o validata."
                       WHEN _codice = 45012
                           THEN "Non e' possibile annullare una prenotazione raggiunti i trenta minuti precedenti l'inizio della proiezione."
                       WHEN _codice = 45013 THEN "Codice prenotazione non valido."
                       WHEN _codice = 45014 THEN "Il cinema selezionato e' invalido o inesistente."
                       WHEN _codice = 45015
                           THEN "Il posto selezionato e' stato gia prenotato per la proiezione selezionata."
                       WHEN _codice = 45016 THEN "Il dipendente selezionato e' invalido o inesistente."
                       WHEN _codice = 45017 THEN "La proiezione selezionata e' invalida o inesistente."
                       WHEN _codice = 45018 THEN "La sala selezionata e' invalida o inesistente."
                       WHEN _codice = 45019 THEN "Il turno selezionato e' invalido o inesistente."
                       WHEN _codice = 45020 THEN "Impossibile confermare una prenotazione non in attesa."
                       WHEN _codice = 45021 THEN "Una prenotazione in attesa puo' essere solamente confermata."
                       WHEN _codice = 45022 THEN "Impossibile completare l'operazione, riprovare in un secondo momento."
                       END);
END
