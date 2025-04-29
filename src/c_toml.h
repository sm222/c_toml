#ifndef  TOML_H
# define TOML_H

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

# define FILE_EXTENSION  ".toml"

# define MAX_FILE_SIZE 10000
# define MAX_VARIABLE  4000

static const char* const errorList[] = {
  "none",
  NULL
};

enum e_Types {
  Invalid = -1  ,
  String        ,
  Integer       ,
  Float         ,
  Boolean       ,
  OffesetTime   ,
  LocalDateTime ,
  LocalDate     ,
  LocalTime     ,
};

enum e_Forms {
  None  = -1 ,
  Variable   ,
  Array      ,
  Table      ,
  InlineTable,
  ArrayTable ,
};

typedef struct C_str {
  size_t len;
  size_t head;
  char*  line;
  bool   lineAlloc;
} C_str;

typedef union u_Types {
  void*         Ptr;
  char*         Str;
  int           Integer;
  float         Float;
  unsigned int  Bool;
} u_Types;

struct Variable {
  int      type;
  int      form; // array, table
  size_t   len;  // if array else alway 1
  size_t   line; // line from raw data
  u_Types* data;
  char*    name;
};

// https://toml.io/en/v1.0.0

typedef struct tomlFile {
  const char* const*  rawData;
  const size_t        fileSize; // byte size
  const size_t        totalLine;
  const char*         fileName; // file name without path
  const char*         filePath; // file path whit name
  // - - - - - - - - - - - - -
  size_t              head;     // reading head (line)
  size_t              headByte; // head + byte (rawData[head][headbyte])
  // - - - - - - - - - - - - -
  int                 error;
  struct Variable     variable;
} tomlFile;

tomlFile   *toml_init(const char* filePath);
int         toml_end(tomlFile* file);

//        -- --  --  --  --  --  --  --

void        toml_info(const tomlFile* file);
int         toml_zero_read(tomlFile* file);

//
const char* toml_readline(tomlFile* file, ssize_t* size);
//

bool        toml_jump_to_line(tomlFile* file, const size_t line);
bool        toml_jump_to_table(tomlFile* file, const char* table);

int         toml_is_file_valid(tomlFile* file);

#endif
