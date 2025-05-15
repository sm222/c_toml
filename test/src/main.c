#include "../../src/c_toml.h"
#include "../../src/toml_utils.h"
#include <string.h>

struct tomlFileEdit {
  char**            rawData;           //
  size_t            fileSize;          // byte size
  size_t            totalLine;         //
  char*             fileName;          // file name with out path
  char*             filePath;          // file path;
  // - - - - - - - - - - - - - - - - - //
  size_t            line;              // reading line (line)
  size_t            cursor;            // line + byte (rawData[line][cursor])
  ssize_t           currentLineLen;    // strlen of (line) after toml_readline -1 line is NULL
  // - - - - - - - - - - - - - - - -   //
  int               error;             //
  t_knowKey*        keysList;          //
  size_t            keysListSize;      //
  // - - - - - - - - - - - - - - - - - //
  char**            refinedData;       //
  size_t            refinedDataSize;   //
};


int test1(void) {
  int errorNb = 0;
  struct tomlFileEdit* file = NULL;
  file = _toml_init_data_("testRun.toml");
  if (file->totalLine != 8) {
    printf("totalLine-> %zu\n", file->totalLine);
    errorNb++;
  }
  const char* q_line[2] = {
    NULL,
    "global e  = true"
  };
  const char  q_char[2] = {
    0,
    '['
  };
  const ssize_t q_lineLen[2] = {
    -1,
    15
  };
  for (size_t i = 0; i < 2; i++) {
    const char* r_line = _toml_current_line(file);
    const char  r_char = _toml_current_char(file);
    const ssize_t r_lineLen = _toml_current_line_len(file);
    printf("[%3zu]_______________________\n", i);
    if (q_line[i] != r_line && strcmp(q_line[i], r_line) != 0) {
      printf("line is not valid:\n" \
        "\tgot %s\n" \
        "\twant %s\n",q_line[i], r_line);
      errorNb++;
    }
    if (q_char[i] != r_char) {
      printf("char is not valid:\n\t>%c\n\t<%c\n",q_char[i], r_char);
      errorNb++;
    }
    if (q_lineLen[i] != r_lineLen) {
      printf("lineLen is not valid:\n\t>%zu\n\t<%zu\n",q_lineLen[i], r_lineLen);
      errorNb++;
    }
    toml_readline(file, NULL);
  }
  _toml_free_file(file);
  printf("total error: %d\n", errorNb);
  return errorNb;
}


//todo add: bool toml_is_file_done(const tomlFile file)
//todo, toml_have(param, type or key)

int test2(void) {
  int errorNb = 0;
  const char* const fileTXT[] = {
    NULL,
    "Test = 1",
    "2",
    "test   = 3",
    "wow   = 4",
    "Test=5",
    "<|> = 6",
    NULL,
  };
  {
    tomlFile file = _toml_make_fake_file(fileTXT + 1);
    _toml_info(file);
    //
    if (file) {
      for (size_t i = 0; i < 8; i++) {
        printf("[%3zu]______________________\n", i);
        const char* readlineResult = _toml_current_line(file);
        //!  line test
        if ((!fileTXT[i] && readlineResult) || (fileTXT[i] && !readlineResult)) {
          errorNb++;
          printf("error:\n>%s\n<%s\n", fileTXT[i], readlineResult);
        }
        else if (readlineResult && fileTXT[i]) {
          if (strcmp(fileTXT[i], readlineResult) != 0) {
            errorNb++;
            printf("error:\n>%s\n<%s\n", fileTXT[i], readlineResult);
          }
        }
        //!            _toml_current_char
        const char c = _toml_current_char(file);
        if (!fileTXT[i] ? '\0' : fileTXT[i][0] != c) {
          char b[2];
          b[0] = c;
          b[1] = 0;
          errorNb++;
          printf("error:\n>%c\n<%s\n", \
            !fileTXT[i] ? '\0' : fileTXT[i][0], c ? b : "\\0");
        }
        const ssize_t lineLen = _toml_current_line_len(file);
        const ssize_t testLineLen = _toml_strlen(fileTXT[i]);
        if (lineLen != testLineLen) {
          errorNb++;
          printf("error: _toml_current_line_len > want\n" \
          "<%zu\n>%zu\n",lineLen, testLineLen);
        }
        //! _toml_current_cursor_index
        if (_toml_current_cursor_index(file) != 0) {
          errorNb++;
          printf("error: want 0, got %zu \n", _toml_current_cursor_index(file));
        }
        ssize_t sizeFromReadline;
        toml_readline(file, &sizeFromReadline);
        const ssize_t sizeFromReadlineEx = _toml_current_line_len(file);
        if (sizeFromReadline != sizeFromReadlineEx) {
          printf("error : size form readline\n>%zu\n<%zu\n", sizeFromReadlineEx, sizeFromReadline);
          errorNb++;
        }
      }
      //
      _toml_free_file(file);
    }
    printf("total error: %d\n", errorNb);
  }
  return errorNb;
}

int main(void) {
  // test init
  //test1();
  test2();
}








/*
global   = true

[abc]

[[hello]]

value = 1
str = "abc"
f = 0.01

#this is a comment
[end]

*/