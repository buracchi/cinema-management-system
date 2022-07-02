import argparse
import os
import pathlib


class SqlScriptGenerator:
    def __init__(self, dbname, in_source_build=False, build_path=None, generate_tables=True, generate_udf=True,
                 generate_functions=True, generate_procedures=True, generate_triggers=True,
                 generate_grants=True, generate_data=True, generate_events=True):
        self.dbname = dbname
        self.generate_tables = generate_tables
        self.generate_udf = generate_udf
        self.generate_functions = generate_functions
        self.generate_procedures = generate_procedures
        self.generate_triggers = generate_triggers
        self.generate_grants = generate_grants
        self.generate_data = generate_data
        self.generate_events = generate_events
        self.base_path = pathlib.Path(__file__).resolve().parent
        self.build_path = self.base_path.joinpath(pathlib.Path('build'))
        if in_source_build:
            self.build_path = pathlib.Path('.')
        if build_path is not None:
            self.build_path = build_path

    @staticmethod
    def _get_comment_header(title):
        return ('-- -----------------------------------------------------\n'
                '-- HEADER_TITLE\n'
                '-- -----------------------------------------------------\n').replace('HEADER_TITLE', title)

    def _absolute_path_of(self, directory):
        return self.base_path.joinpath(directory)

    def _get_schema_section(self):
        result = ''
        result += '-- Generated by ' + pathlib.Path(__file__).name + '\n'
        result += '\n'
        result += 'SET @OLD_UNIQUE_CHECKS = @@UNIQUE_CHECKS, UNIQUE_CHECKS = 0;\n'
        result += 'SET @OLD_FOREIGN_KEY_CHECKS = @@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS = 0;\n'
        result += 'SET @OLD_SQL_MODE = @@SQL_MODE, SQL_MODE =\n'
        result += '        '
        result += "'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,"
        result += "ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';\n"
        result += '\n'
        result += self._get_comment_header('Schema ' + self.dbname)
        result += '\n'
        result += "DROP SCHEMA IF EXISTS `" + self.dbname + "`;\n"
        result += 'CREATE SCHEMA IF NOT EXISTS `' + self.dbname + '` DEFAULT CHARACTER SET utf8mb4;\n'
        result += 'USE `' + self.dbname + '`;\n'
        result += '\n'
        return result

    def _get_tables_section(self):
        result = ''
        for table_name in ['Cinema', 'Sale', 'Posti', 'Ruoli', 'Dipendenti', 'Giorni', 'Turni', 'Film',
                           'Proiezioni', 'Stati_Prenotazione', 'Prenotazioni']:
            result += self._get_comment_header('Table `' + self.dbname + '`.`' + table_name + '`')
            result += '\n'
            with open(self._absolute_path_of('tables/' + table_name + '.sql'), 'r') as source:
                result += source.read()
            result += '\n'
        return result

    def _get_udf_section(self):
        result = self._get_comment_header('UDF Loadable Functions')
        result += '\n'
        with open(self._absolute_path_of('udf/payment_service.sql'), 'r') as source:
            result += source.read()
        result += '\n'
        if os.name != 'nt':
            return result.replace('.dll', '.so')
        return result

    def _get_functions_section(self):
        result = ''
        for function in self._absolute_path_of('functions').iterdir():
            result += self._get_comment_header('Function `' + self.dbname + '`.`' + function.stem + '`')
            result += '\n'
            result += 'DELIMITER $$\n'
            result += 'USE `' + self.dbname + '`$$\n'
            with open(function, 'r') as source:
                result += source.read()
            result = result[:-1] + '$$\n'
            result += '\n'
            result += 'DELIMITER ;\n'
            result += '\n'
        return result

    def _get_procedures_section(self):
        result = ''
        for procedure in self._absolute_path_of('procedures').iterdir():
            result += self._get_comment_header('Procedure `' + self.dbname + '`.`' + procedure.stem + '`')
            result += '\n'
            result += 'DELIMITER $$\n'
            result += 'USE `' + self.dbname + '`$$\n'
            with open(procedure, 'r') as source:
                result += source.read()
            result = result[:-1] + '$$\n'
            result += '\n'
            result += 'DELIMITER ;\n'
            result += '\n'
        return result

    def _get_triggers_section(self):
        result = ''
        for trigger in self._absolute_path_of('triggers').iterdir():
            result += self._get_comment_header('Trigger `' + self.dbname + '`.`' + trigger.stem + '`')
            result += '\n'
            result += 'DELIMITER $$\n'
            result += 'USE `' + self.dbname + '`$$\n'
            with open(trigger, 'r') as source:
                result += source.read()
            result = result[:-1] + '$$\n'
            result += '\n'
        return result

    def _get_grants_section(self):
        result = self._get_comment_header('Privileges Grant')
        result += '\n'
        result += 'DELIMITER ;\n'
        with open(self._absolute_path_of('grants/grants.sql'), 'r') as source:
            result += source.read()
        result += '\n'
        return result

    def _get_data_section(self):
        result = ''
        for table_name in ['Cinema', 'Ruoli', 'Giorni', 'Film', 'Stati_Prenotazione', 'Sale', 'Posti',
                           'Dipendenti', 'Proiezioni', 'Turni']:
            result += self._get_comment_header('Data for table `' + self.dbname + '`.`' + table_name + '`')
            result += '\n'
            with open(self._absolute_path_of('data/' + table_name + '.sql'), 'r') as source:
                result += source.read()
            result += '\n'
        return result

    def _get_events_section(self):
        result = ''
        for event in pathlib.Path('events').iterdir():
            result += '\n'
            result += self._get_comment_header('Event `' + self.dbname + '`.`' + event.stem + '`')
            result += '\n'
            with open(self._absolute_path_of(event), 'r') as source:
                result += source.read()
            result += '\n'
        return result

    def generate_sql_script(self):
        self.build_path.mkdir(exist_ok=True)
        with open(self.build_path.joinpath(self.dbname + '.sql'), 'w') as sql_script:
            sql_script.write(self._get_schema_section())
            if self.generate_tables:
                sql_script.write(self._get_tables_section())
            sql_script.write('USE ' + '`' + self.dbname + '`;\n')
            sql_script.write('\n')
            if self.generate_udf:
                sql_script.write(self._get_udf_section())
            if self.generate_functions:
                sql_script.write(self._get_functions_section())
            if self.generate_procedures:
                sql_script.write(self._get_procedures_section())
            if self.generate_triggers:
                sql_script.write(self._get_triggers_section())
            if self.generate_grants:
                sql_script.write(self._get_grants_section())
            sql_script.write('SET SQL_MODE=@OLD_SQL_MODE;\n')
            sql_script.write('SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;\n')
            sql_script.write('SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;\n')
            sql_script.write('\n')
            if self.generate_data:
                sql_script.write(self._get_data_section())
            if self.generate_events:
                sql_script.write('SET GLOBAL event_scheduler = ON;\n')
                sql_script.write(self._get_events_section()[:-1])


if __name__ == '__main__':
    database_name = 'cinemadb'
    parser = argparse.ArgumentParser(description=database_name + ' SQL script generator builder.')
    build_path_group = parser.add_argument_group('build path')
    build_path_group.add_argument('--in-source-build', action=argparse.BooleanOptionalAction, default=False,
                                  help='the generator script will be created in the current directory '
                                       'instead of the standard build directory if the BUIL_PATH option'
                                       'is left unspecified, otherwise this option have no effect.')
    build_path_group.add_argument('-b', '--build-path',
                                  help='Specify the output directory of the database generator script.')
    sections_group = parser.add_argument_group('script sections')
    sections_group.add_argument('--tables-section', action=argparse.BooleanOptionalAction, default=True,
                                help='include tables generation section.')
    sections_group.add_argument('--udf-section', action=argparse.BooleanOptionalAction, default=True,
                                help='include loadable functions (udf) generation section.')
    sections_group.add_argument('--functions-section', action=argparse.BooleanOptionalAction, default=True,
                                help='include native functions generation section.')
    sections_group.add_argument('--procedures-section', action=argparse.BooleanOptionalAction, default=True,
                                help='include procedures generation section.')
    sections_group.add_argument('--triggers-section', action=argparse.BooleanOptionalAction, default=True,
                                help='include triggers generation section.')
    sections_group.add_argument('--grants-section', action=argparse.BooleanOptionalAction, default=True,
                                help='include grants generation section.')
    sections_group.add_argument('--data-section', action=argparse.BooleanOptionalAction, default=True,
                                help='include data generation section.')
    sections_group.add_argument('--events-section', action=argparse.BooleanOptionalAction, default=True,
                                help='include events generation section.')
    args = parser.parse_args()
    sql_script_generator = SqlScriptGenerator(
        dbname=database_name,
        in_source_build=args.in_source_build,
        build_path=pathlib.Path(args.build_path) if args.build_path is not None else None,
        generate_tables=args.tables_section,
        generate_udf=args.udf_section,
        generate_functions=args.functions_section,
        generate_procedures=args.procedures_section,
        generate_triggers=args.triggers_section,
        generate_grants=args.grants_section,
        generate_data=args.data_section,
        generate_events=args.events_section)
    sql_script_generator.generate_sql_script()
