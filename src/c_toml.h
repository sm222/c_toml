#ifndef  TOML_H
# define TOML_H

# ifndef  VERSION
#  define VERSION
#  define VERSION_MAJOR 0
#  define VERSION_MINOR 1
#  define VERSION_PATCH 0
# endif

// https://toml.io/en/v1.0.0


# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include "toml_types.h"

typedef enum {
  None,
  No_file,
  Empty_file,
  Invalid_file,
  Invalid_variable,
  invalid_table,
} e_error;


static const char* const errorList[] = {
  "none - all good to go",
  "no file",
  "empty file",
  "invalid file",
  "invalid variable",
  "invalid table",
  NULL
};


# define ERROR_TYPE(errorValue) \
(X_IN_Y(errorValue, GET_ARRAY_SIZE(errorList))) ? errorList[errorValue] : "invalid value!"


const char*       toml_version(void);

tomlFile*         toml_init(const char* filePath);
int               toml_is_file_valid(tomlFile* file);

//        -- --  --  --  --  --  --  --

void              toml_info(const tomlFile* file);
int               toml_zero_read(tomlFile* file);

//
const char*       toml_readline(tomlFile* file, ssize_t* size);
//

//Todo
bool              toml_jump_to_line(tomlFile* file, const size_t line);
bool              toml_jump_to_table(tomlFile* file, const char* table);
bool              toml_jump_to_key(  tomlFile* file, const char* key);
t_field*          toml_get_value_type(tomlFile* file);

int               toml_end(tomlFile* file);

#endif
