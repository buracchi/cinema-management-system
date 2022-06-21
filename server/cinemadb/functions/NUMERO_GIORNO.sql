CREATE FUNCTION `NUMERO_GIORNO` (_nome VARCHAR(15))
RETURNS TINYINT
DETERMINISTIC
READS SQL DATA
BEGIN
	RETURN (SELECT `numero`
			FROM `Giorni`
            WHERE `nome` = _nome);
END
