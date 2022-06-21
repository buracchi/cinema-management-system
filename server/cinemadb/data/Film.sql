START TRANSACTION;
USE `cinemadb`;
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`)
VALUES (DEFAULT, 'Il padrino', '02:55:00', 'Paramount Pictures Studios, ALTRO',
        'Marlon Brando, Al Pacino, James Caan, Richard S. Castellano, Robert Duvall, Diane Keaton, John Cazale, Talia Shire, Abe Vigoda, Al Lettieri, Gianni Russo, Lenny Montana');
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`)
VALUES (DEFAULT, 'Il cavaliere oscuro', '02:32:00', 'Warner Bros. Pictures, ALTRO',
        'Christian Bale, Cillian Murphy, Gary Oldman, Morgan Freeman, Heath Ledger, Michael Caine, Maggie Gyllenhaal, Aaron Eckhart');
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`)
VALUES (DEFAULT, 'Frankenstein Junior', '01:46:00', 'Gruskoff/Venture Films, ALTRO',
        'Gene Wilder, Peter Boyle, Marty Feldman, Teri Garr, Cloris Leachman');
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`)
VALUES (DEFAULT, 'Pulp Fiction', '02:34:00', 'Miramax, ALTRO', NULL);
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`)
VALUES (DEFAULT, 'Il buono, il brutto, il cattivo', '02:41:00', 'PEA', NULL);
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`)
VALUES (DEFAULT, 'Fight Club', '02:19:00', 'Linson Films', NULL);
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`)
VALUES (DEFAULT, 'Matrix', '02:16:00', 'Warner Bros., ALTRO', NULL);
INSERT INTO `cinemadb`.`Film` (`id`, `nome`, `durata`, `casa_cinematografica`, `cast`)
VALUES (DEFAULT, 'Amici miei', '02:20:00', 'Rizzoli Film, ALTRO', NULL);

COMMIT;
