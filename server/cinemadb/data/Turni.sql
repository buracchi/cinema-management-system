START TRANSACTION;
INSERT INTO `Turni` (`dipendente`, `giorno`, `inizio`, `durata`, `cinema`)
SELECT D.matricola, Giorni.nome AS giorno, Orari.inizio, Orari.durata, Cinema.id AS cinema
FROM (SELECT matricola FROM Dipendenti WHERE Dipendenti.ruolo = 'Maschera' ORDER BY matricola ASC LIMIT 20) AS D
         JOIN Giorni
         CROSS JOIN (SELECT '10:00:00' AS inizio, '05:00:00' AS durata) AS Orari
         JOIN Cinema ON MOD(matricola, (SELECT COUNT(*) FROM Cinema)) = Cinema.id
UNION ALL
SELECT D.matricola, Giorni.nome AS giorno, Orari.inizio, Orari.durata, Cinema.id AS cinema
FROM (SELECT matricola FROM Dipendenti WHERE Dipendenti.ruolo = 'Maschera' ORDER BY matricola DESC LIMIT 20) AS D
         JOIN Giorni
         CROSS JOIN (SELECT '15:00:00' AS inizio, '08:00:00' AS durata) AS Orari
         JOIN Cinema ON MOD(matricola, (SELECT COUNT(*) FROM Cinema)) = Cinema.id
UNION ALL
SELECT D.matricola, Giorni.nome AS giorno, Orari.inizio, Orari.durata, Cinema.id AS cinema
FROM (SELECT matricola FROM Dipendenti WHERE Dipendenti.ruolo = 'Proiezionista' ORDER BY matricola ASC LIMIT 50) AS D
         JOIN Giorni
         CROSS JOIN (SELECT '10:00:00' AS inizio, '05:00:00' AS durata) AS Orari
         JOIN Cinema ON MOD(matricola, (SELECT COUNT(*) FROM Cinema)) = Cinema.id
UNION ALL
SELECT D.matricola, Giorni.nome AS giorno, Orari.inizio, Orari.durata, Cinema.id AS cinema
FROM (SELECT matricola FROM Dipendenti WHERE Dipendenti.ruolo = 'Proiezionista' ORDER BY matricola DESC LIMIT 20) AS D
         JOIN Giorni
         CROSS JOIN (SELECT '15:00:00' AS inizio, '08:00:00' AS durata) AS Orari
         JOIN Cinema ON MOD(matricola, (SELECT COUNT(*) FROM Cinema)) = Cinema.id
ORDER BY matricola ASC, cinema ASC;

COMMIT;
