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

# define VALID_SPACE "\t "
# define VALID_NEXT_LINE '\r'
# define COMMENT     '#'

static const char* const errorList[] = {
  "none",
  NULL
};

typedef struct c_str {
  size_t len;
  size_t head;
  char*  line;
  bool   lineAlloc;
} c_str;

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
}  tomlFile;

tomlFile   *toml_init(const char* filePath);
int         toml_end(tomlFile* file);

//        -- --  --  --  --  --  --  --

void        toml_info(const tomlFile* file);

//
const char* toml_readline(tomlFile* file, ssize_t* size);

#endif
