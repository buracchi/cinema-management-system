DROP EVENT IF EXISTS `cinemadb`.`scadenza_prenotazioni`;
CREATE EVENT `cinemadb`.`scadenza_prenotazioni`
ON SCHEDULE EVERY 1 MINUTE
STARTS TIMESTAMP(CURDATE())
DO
    UPDATE `Prenotazioni` SET `stato`='Scaduta'
    WHERE `codice` IN (SELECT `codice`
						FROM `Prenotazioni` JOIN `Proiezioni` ON `Prenotazioni`.`cinema` = `Proiezioni`.`cinema`
																AND `Prenotazioni`.`sala` = `Proiezioni`.`sala`
																AND `Prenotazioni`.`data` = `Proiezioni`.`data`
																AND `Prenotazioni`.`ora` = `Proiezioni`.`ora`
											JOIN `Film` ON `film` = `id`
						WHERE `stato` = 'Confermata'
							AND CURRENT_TIMESTAMP() > TIMESTAMP(`Prenotazioni`.`data`,
								SEC_TO_TIME(TIME_TO_SEC(`Proiezioni`.`ora`) + TIME_TO_SEC(`durata`))));
