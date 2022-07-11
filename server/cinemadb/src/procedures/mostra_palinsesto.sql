CREATE PROCEDURE `mostra_palinsesto`(IN _cinema_id INT)
BEGIN
    DECLARE _cinema_inesistente CONDITION FOR SQLSTATE '45014';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45014);
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            RESIGNAL;
        END;
    SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
    START TRANSACTION;
    IF (_cinema_id NOT IN (SELECT `id` FROM `Cinema`)) THEN
        SIGNAL _cinema_inesistente SET MESSAGE_TEXT = _err_msg;
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
    COMMIT;
END
