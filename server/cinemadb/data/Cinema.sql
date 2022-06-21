START TRANSACTION;
USE `cinemadb`;
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (1, 'Via Giuseppe Verdi 72100 BR Brindisi', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (2, 'Via Roma 12100 CN Cremona', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (3, 'Via Dante 40100 BO Bologna', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (4, 'Via Cavour 1810 IM Imperia', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (5, 'Via Monte Bianco 94100 EN Enna', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (6, 'Via Giove 28100 NO Novara', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (7, 'Via della Repubblica RM 00100 Roma', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (8, 'Via de Gasperi 33170 PN Pordenone', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (9, 'Via dei Tulipani 37100 VR Verona', '10:00:00', '23:00:00');
INSERT INTO `cinemadb`.`Cinema` (`id`, `indirizzo`, `apertura`, `chiusura`) VALUES (10, 'Via dei Frati 92100 AO Aosta', '10:00:00', '23:00:00');

COMMIT;
