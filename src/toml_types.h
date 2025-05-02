#ifndef  TOML_TYPES_H
# define TOML_TYPES_H

# include <stdio.h>

# ifndef  TXT_COLORS
#  define TXT_COLORS
#  define CS	"\001" //*start code
#  define CE	"\002" //*end   code
#  define TXT_RED	CS "\e[31m" CE
#  define TXT_GRN	CS "\e[32m" CE
#  define TXT_YEL	CS "\e[33m" CE
#  define TXT_BLU	CS "\e[34m" CE
#  define TXT_MAG	CS "\e[35m" CE
#  define TXT_CYN	CS "\e[36m" CE
#  define TXT_WHT	CS "\e[37m" CE
#  define TXT_ORG	CS "\e[38;5;202m"  CE
#  define TXT_PIK	CS "\e[38;5;176m"  CE
#  define TXT_RESET	CS "\e[0m\022"   CE
#  define TXT_CLE	CS "\e[1;1H\x1b[2J" CE
# endif

typedef enum {
  Invalid =   -1,
  Field         ,
  Table         ,
  String        ,
  Integer       ,
  Float         ,
  Boolean       ,
  OffesetTime   ,
  LocalDateTime ,
  LocalDate     ,
  LocalTime     ,
} e_types;

# define X_IN_Y(x, y)    (x < y && x >= 0)

static const char* const KeyValueTypes[] = {
  "Invalid!",
  "Field",
  "Table",
  "String",
  "Integer",
  "Float",
  "Boolean",
  "OffesetTime",
  "LocalDateTime",
  "LocalDate",
  "LocalTime",
  NULL
};

static inline size_t _toml_get_size_array(const char* const* array) {
  if (!array)
    return 0;
  size_t i = 0;
  while (array[i]) { i++; }
  return i;
}

# define GET_ARRAY_SIZE(array)  _toml_get_size_array(array)

# define KEY_VALUE(types) \
(X_IN_Y(types, GET_ARRAY_SIZE(KeyValueTypes))) ? KeyValueTypes[types] : "out of bound"

typedef struct {
  char*       key;
  e_types     type;
  void*       value;
  size_t      amount;
} t_field;

typedef struct {
  t_field*  fields;
  char*     tableName;
  size_t    fieldAmount;
} t_table;

struct dict {
  char*    key;
  t_field  type;
  e_types  value;
};

typedef struct {
  char*   key;
  size_t  line;
} t_knowKey;


typedef struct tomlFile {
  const char* const*  rawData;
  const size_t        fileSize; // byte size
  const size_t        totalLine;
  const char* const   fileName; // file name without path
  const char* const   filePath; // file path whit name
  // - - - - - - - - - - - - -
  size_t              line;     // reading line (line)
  size_t              cursor; // line + byte (rawData[line][cursor])
  // - - - - - - - - - - - - -
  int                 error;
  t_knowKey*          keysList;
  size_t              keysListSize;
} tomlFile;

#endif
