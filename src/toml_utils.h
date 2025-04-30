#ifndef  TOML_UTILS_H
# define TOML_UTILS_H

# include <unistd.h>
# include <stdio.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>

#ifndef FILE_SEP
# define FILE_SEP '/'
#endif

#ifndef FILE_LINE_SEP
# define FILE_LINE_SEP '\n'
#endif

# define VALID_SPACE     "\t "
# define VALID_NEXT_LINE '\r'
# define COMMENT         '#'

# define TABLE_SIMBOLE   "[]"
# define TABLE_OPEN      0
# define TABLE_CLOSE     1

# define VALID_VAR_NAME_SECSION 3
# define VALID_VAR_NAME "azAZ09"

# define FILE_EXTENSION  ".toml"

# define MAX_FILE_SIZE 10000
# define MAX_VARIABLE  4000

struct tomlFileEdit {
    char**          rawData;
    size_t          fileSize; // byte size
    size_t          totalLine;
    char*           fileName; // file name with out path
    char*           filePath; // file path;
    // - - - - - - - - - -
    size_t          head;     // reading head (line)
    size_t          headByte; // head + byte (rawData[head][headbyte])
    // - - - - - - - - - - - - -
    int             error;
};

//void _toml_set_name(struct tomlFileEdit* file, const char* filePath);


#endif