DROP EVENT IF EXISTS `cinemadb`.`pulizia_prenotazioni`;
CREATE EVENT `cinemadb`.`pulizia_prenotazioni`
ON SCHEDULE EVERY 1 MONTH
STARTS TIMESTAMP(DATE_FORMAT(DATE_ADD(CURDATE(), INTERVAL 1 MONTH), '%Y-%m-1'))
DO
	DELETE FROM `Prenotazioni` WHERE `data` < CURDATE();
