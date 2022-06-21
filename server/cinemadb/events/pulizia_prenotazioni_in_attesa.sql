DROP EVENT IF EXISTS `cinemadb`.`pulizia_prenotazioni_in_attesa`;
CREATE EVENT `cinemadb`.`pulizia_prenotazioni_in_attesa`
ON SCHEDULE EVERY 1 MINUTE
STARTS TIMESTAMP(CURDATE())
DO
	DELETE FROM `Prenotazioni`
			WHERE `stato`='Attesa'
				AND `timestamp` < DATE_SUB(NOW(), INTERVAL 10 MINUTE);
