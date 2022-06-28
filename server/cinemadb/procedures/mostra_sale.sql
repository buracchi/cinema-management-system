CREATE PROCEDURE `mostra_sale`(IN _cinema INT)
BEGIN
    DECLARE _cinema_inesistente CONDITION FOR SQLSTATE '45014';
    DECLARE _err_msg VARCHAR(128) DEFAULT MESSAGGIO_ERRORE(45014);
    IF (_cinema NOT IN (SELECT `id` FROM `Cinema`)) THEN
        SIGNAL _cinema_inesistente SET MESSAGE_TEXT = _err_msg;
    END IF;
    SELECT `sala`,
           COUNT(DISTINCT fila)   AS numero_file,
           COUNT(DISTINCT numero) AS posti_per_fila
    FROM `Posti`
    WHERE `cinema` = _cinema
    GROUP BY `sala`;
END
