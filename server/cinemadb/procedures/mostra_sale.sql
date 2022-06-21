CREATE PROCEDURE `mostra_sale`(IN _cinema INT)
BEGIN
    IF (_cinema NOT IN (SELECT `id` FROM `Cinema`)) THEN
        SET @err_msg = MESSAGGIO_ERRORE(45014);
        SIGNAL SQLSTATE '45014'
            SET MESSAGE_TEXT = @err_msg;
    END IF;
    SELECT `sala`,
           COUNT(DISTINCT fila)   AS numero_file,
           COUNT(DISTINCT numero) AS posti_per_fila
    FROM `Posti`
    WHERE `cinema` = _cinema
    GROUP BY `sala`;
END
