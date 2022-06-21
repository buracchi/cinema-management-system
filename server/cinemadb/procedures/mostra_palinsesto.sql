CREATE PROCEDURE `mostra_palinsesto`(IN _cinema_id INT)
BEGIN
    IF (_cinema_id NOT IN (SELECT `id` FROM `Cinema`)) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45014);
        SIGNAL SQLSTATE '45014'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
    SELECT `data`,
           `ora`,
           `cinema`,
           `sala`,
           `prezzo`,
           `nome`,
           `durata`,
           `casa_cinematografica`,
           `cast`
    FROM `Proiezioni`
             JOIN `Film` ON `film` = `id`
    WHERE `cinema` = _cinema_id
      AND (`data` > CURDATE()
        OR (`data` = CURDATE() AND `ora` > TIME(NOW())))
    ORDER BY `data`, `ora`, `cinema`, `sala`;
END
