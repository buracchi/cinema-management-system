CREATE PROCEDURE `mostra_turni`()
BEGIN
    SELECT `giorno`,
           `inizio`,
           `durata`,
           `cinema`,
           `indirizzo`,
           `matricola`,
           `nome`,
           `cognome`,
           `ruolo`
    FROM `Turni`
             JOIN `Dipendenti`
             JOIN `Cinema` ON `cinema` = `id`
    WHERE `dipendente` = `matricola`;
END
