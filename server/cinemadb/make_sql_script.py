import pathlib


def generate_sql_script(in_source_build=False):
    dbname = 'cinemadb'
    if in_source_build:
        build_path = pathlib.Path('.')
    else:
        build_path = pathlib.Path(__file__).cwd().joinpath(pathlib.Path('build'))
        build_path.mkdir(exist_ok=True)
    with open(build_path.joinpath(dbname + '.sql'), 'w') as sql_script:

        sql_script.write('-- ' + pathlib.Path(__file__).name + ' Forward Engineering\n\n')

        with open('utilities/checks.sql', 'r') as source:
            sql_script.writelines(source.readlines())
        sql_script.write('\n')

        sql_script.write('-- -----------------------------------------------------\n')
        sql_script.write('-- Schema ' + dbname + '\n')
        sql_script.write('-- -----------------------------------------------------\n')
        with open('utilities/schema.sql', 'r') as source:
            sql_script.writelines(source.readlines())
        sql_script.write('\n')

        for table_name in ['Cinema', 'Sale', 'Posti', 'Ruoli', 'Dipendenti', 'Giorni', 'Turni', 'Film',
                           'Proiezioni', 'StatiPrenotazione', 'Prenotazioni']:
            with open('tables/' + table_name + '.sql', 'r') as source:
                sql_script.write('-- -----------------------------------------------------\n')
                sql_script.write('-- Table `' + dbname + '`.`' + table_name + '`\n')
                sql_script.write('-- -----------------------------------------------------\n')
                sql_script.write('\n')
                sql_script.writelines(source.readlines())
                sql_script.write('\n')
        sql_script.write('USE ' + '`' + dbname + '`;\n')

        sql_script.write('-- begin \'UDF\' Loadable Functions script\n\n')
        with open('udf/payment_service.sql', 'r') as source:
            sql_script.writelines(source.readlines())
        sql_script.write('\n')

        for function in pathlib.Path('functions').iterdir():
            with open(function, 'r') as source:
                sql_script.write('-- -----------------------------------------------------\n')
                sql_script.write('-- Function `' + dbname + '`.`' + function.stem + '`\n')
                sql_script.write('-- -----------------------------------------------------\n')
                sql_script.write('\n')
                sql_script.write('DELIMITER $$\n')
                sql_script.write('USE `cinemadb`$$\n')
                sql_script.writelines(source.readlines())
                sql_script.seek(sql_script.tell() - 2, 0)
                sql_script.write('$$\n')
                sql_script.write('\n')
                sql_script.write('DELIMITER ;\n')
                sql_script.write('\n')

        for trigger in pathlib.Path('procedures').iterdir():
            with open(trigger, 'r') as source:
                sql_script.write('-- -----------------------------------------------------\n')
                sql_script.write('-- Procedure `' + dbname + '`.`' + trigger.stem + '`\n')
                sql_script.write('-- -----------------------------------------------------\n')
                sql_script.write('\n')
                sql_script.write('DELIMITER $$\n')
                sql_script.write('USE `cinemadb`$$\n')
                sql_script.writelines(source.readlines())
                sql_script.seek(sql_script.tell() - 2, 0)
                sql_script.write('$$\n')
                sql_script.write('\n')
                sql_script.write('DELIMITER ;\n')
                sql_script.write('\n')

        for trigger in pathlib.Path('triggers').iterdir():
            with open(trigger, 'r') as source:
                sql_script.write('-- -----------------------------------------------------\n')
                sql_script.write('-- Trigger `' + dbname + '`.`' + trigger.stem + '`\n')
                sql_script.write('-- -----------------------------------------------------\n')
                sql_script.write('\n')
                sql_script.write('DELIMITER $$\n')
                sql_script.write('USE `cinemadb`$$\n')
                sql_script.writelines(source.readlines())
                sql_script.seek(sql_script.tell() - 2, 0)
                sql_script.write('$$\n')
                sql_script.write('\n')

        with open('grants/grants.sql', 'r') as source:
            sql_script.write('-- -----------------------------------------------------\n')
            sql_script.write('-- Grants\n')
            sql_script.write('-- -----------------------------------------------------\n')
            sql_script.write('\n')
            sql_script.write('DELIMITER ;\n')
            sql_script.writelines(source.readlines())
            sql_script.write('\n')
            sql_script.write('SET SQL_MODE=@OLD_SQL_MODE;\n')
            sql_script.write('SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;\n')
            sql_script.write('SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;\n')
        sql_script.write('\n')

        for table_name in ['Cinema', 'Ruoli', 'Giorni', 'Film', 'StatiPrenotazione', 'Sale', 'Posti',
                           'Dipendenti', 'Proiezioni', 'Turni']:
            with open('data/' + table_name + '.sql', 'r') as source:
                sql_script.write('-- -----------------------------------------------------\n')
                sql_script.write('-- Data for table `' + dbname + '`.`' + table_name + '`\n')
                sql_script.write('-- -----------------------------------------------------\n')
                sql_script.write('\n')
                sql_script.writelines(source.readlines())
                sql_script.write('\n')

        sql_script.write('SET GLOBAL event_scheduler = ON;\n')
        for event in pathlib.Path('events').iterdir():
            sql_script.write('\n')
            with open(event, 'r') as source:
                sql_script.write('-- -----------------------------------------------------\n')
                sql_script.write('-- Event `' + dbname + '`.`' + event.stem + '`\n')
                sql_script.write('-- -----------------------------------------------------\n')
                sql_script.write('\n')
                sql_script.writelines(source.readlines())
                sql_script.write('\n')

        sql_script.seek(sql_script.tell() - 2, 0)
        sql_script.truncate()


if __name__ == '__main__':
    generate_sql_script()
