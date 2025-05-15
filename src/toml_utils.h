#ifndef  TOML_UTILS_H
# define TOML_UTILS_H

# include <unistd.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include "toml_types.h"

# define LINE_OF_SET(line)   (line - 1)


#ifndef FILE_SEP
# define FILE_SEP '/'
#endif

#ifndef FILE_LINE_SEP
# define FILE_LINE_SEP '\n'
#endif

# define VALID_SPACE        "\t "
# define VALID_NEXT_LINE    '\r'
# define COMMENT            '#'

# define QUOTATION_SYMBOLS  "'\""
# define Q_SINGLE             0
# define Q_DOUBLE             1


# define TABLE_SIMBOLE   "[]"
# define TABLE_OPEN      0
# define TABLE_CLOSE     1

# define VALID_VAR_NAME_SECSION 3
# define VALID_VAR_NAME "azAZ09"

# define FILE_EXTENSION  ".toml"

# define MAX_FILE_SIZE 10000
# define MAX_VARIABLE  4000
# define MAX_LINE_LEN  1000

//************************************/
// toml function for the lib         */
// user can but SHOULD NOT use those */
//************************************/


//********************************/
//              Start            */
//********************************/

tomlFile   _toml_init_data_(const char* filePath);

void       _toml_read_file(tomlFile file);
void       _toml_set_name(tomlFile  file, const char* filePath);

//


//********************************/
//             debug             */
//********************************/

void      _toml_info(const tomlFile file);
void      _toml_print_l(const tomlFile file);
int       _toml_get_error(const tomlFile file);
void      _toml_print_error_parsing(const tomlFile file);
tomlFile  _toml_make_fake_file(const char* const* txt);


//********************************/
//            parsing            */
//********************************/

//

ssize_t     _toml_skip_spaces(tomlFile file);

//

bool        _toml_file_done_reading(const tomlFile file);      //todo
bool        _toml_file_done_reading_line(const tomlFile file); //todo
const char* _toml_read_line(tomlFile file, ssize_t* size);
ssize_t     _toml_get_file_line_number(const tomlFile file);

//

ssize_t     _toml_get_file_byte_size(const tomlFile file);

//

const char* _toml_current_line(const tomlFile file);
char        _toml_current_char(const tomlFile file);
ssize_t     _toml_current_line_len(const tomlFile file);
ssize_t     _toml_current_line_index(const tomlFile file);
ssize_t     _toml_current_cursor_index(const tomlFile file);
int         _toml_is_in_quotation(const tomlFile file, ssize_t i);


//********************************/
//              var              */
//********************************/


int        _toml_get_name(tomlFile file);
//todo     ^ rename

int      _toml_zero_read(tomlFile file, int mode);
int      _toml_add_to_read(tomlFile file, int mode, int ammout);
void     _toml_set_readLine_len(tomlFile file ,ssize_t len);

t_table* _toml_make_tables(const char* name, t_field* fields, size_t fieldAmount);
t_table* _toml_make_default_table(const char* name);

t_field* _toml_make_field(const char* key, e_types type, void* value, size_t ammout);
t_field* _toml_make_default_field(const char* key);

//********************************/
//              end              */
//********************************/

void      _toml_free_file(tomlFile file);

#endif