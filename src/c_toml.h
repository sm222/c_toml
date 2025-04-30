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



static const char* const errorList[] = {
  "none",
  NULL
};


/// @brief use for variable find in toml file
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


static const char* const KeyValueTypes[] = {
  "Invalid"       ,
  "String"        ,
  "Integer"       ,
  "Float"         ,
  "Boolean"       ,
  "OffesetTime"   ,
  "LocalDateTime" ,
  "LocalDate"     ,
  "LocalTime"     ,
  NULL
};

# define VARIBLE(x)    KeyValueTypes[x]

enum e_Forms {
  None  = -1 ,
  Variable   ,
  Array      ,
  Table      ,
  InlineTable,
  ArrayTable ,
};

static const char* const FormatTypes[] = {
  "None",
  "Variable",
  "Array",
  "Table",
  "InlineTable",
  "ArrayTAble",
  NULL
};

# define FORMAT(x)   FormatTypes[x + 1]

typedef struct C_str {
  size_t len;
  char*  line;
  bool   lineAlloc;
} C_str;

/// @brief use for union
static const char* const VariableTypes[] = {
  "ptr",
  "Str",
  "Integer",
  "Float",
  "Bool",
  NULL
};

# define VARIABLE(x)     VariableTypes[x]

typedef union u_Types {
  void*         Ptr;
  char*         Str;
  int           Integer;
  float         Float;
  unsigned int  Bool;
} u_Types;

struct dict {
  char*   key;
  int     type;
  u_Types value;
};


struct Variable {
  int      type;
  int      form; // array, table
  size_t   len;  // if array else alway 1
  size_t   line; // line from raw data
  u_Types* data;
  char*    name;
};


typedef struct tomlFile {
  const char* const*  rawData;
  const size_t        fileSize; // byte size
  const size_t        totalLine;
  const char* const   fileName; // file name without path
  const char* const   filePath; // file path whit name
  // - - - - - - - - - - - - -
  size_t              head;     // reading head (line)
  size_t              headByte; // head + byte (rawData[head][headbyte])
  // - - - - - - - - - - - - -
  int                 error;
} tomlFile;

const char* const toml_version(void);

tomlFile*         toml_init(const char* filePath);
int               toml_end(tomlFile* file);

//        -- --  --  --  --  --  --  --

void              toml_info(const tomlFile* file);
int               toml_zero_read(tomlFile* file);

//
const char*       toml_readline(tomlFile* file, ssize_t* size);
//

bool              toml_jump_to_line(tomlFile* file, const size_t line);
bool              toml_jump_to_table(tomlFile* file, const char* table);
bool              toml_jump_to_key(  tomlFile* file, const char* key);
struct Variable*  toml_get_value_type(tomlFile* file);

int               toml_is_file_valid(tomlFile* file);

#endif
