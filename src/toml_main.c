
#include "c_toml.h"
#include "toml_utils.h"
#include "toml_types.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>


const char*    toml_version(void) {
  static char s[31];
  #ifdef VERSION
  snprintf(s, 30, "c_toml version:%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
  #else
  snprintf(s, 20, "c_toml version: UNDEFINE");
  #endif
  return s;
}

void  _toml_print_l(tomlFile* file) {
  if (!file)
    return;
  const char* l = file->rawData[LINE_OF_SET(file->line)];
  printf("%zu:%zu [%zu]\n%s\n%s\n", strlen(l), file->cursor, file->cursor ,l, file->cursor < (size_t)file->currentLen ? l : ":<- end");
  printf("____________________________\n");
}

int toml_zero_read(tomlFile* file) {
  return _toml_zero_read(file, 3);
}

/// @brief 
/// @param filePath 
/// @return only time it return null is if calloc fail
tomlFile* toml_init(const char* filePath) {
  return (tomlFile*)_toml_init_data_(filePath);
}


int toml_end(tomlFile* file) {
  if (!file)
    return 1;
  _toml_free_file(file);
  return 0;
}


void toml_info(const tomlFile* file) {
  if (!file)
    return ;
  const int size = 1001;
  char      str[size];
  snprintf(str, size -1,"name: %s\n\tpath: %s\n"  \
    "\tsize: %zu\n\ttotal line: %zu\n\terror %d\n"\
    "line: %zu\ncursor: %zu\n", file->fileName,   \
    file->filePath, file->fileSize, file->totalLine, file->error, file->line, file->cursor);
  write(1, str, strlen(str));
}


const char* _toml_read_line(tomlFile* file, ssize_t* size) {
  _toml_zero_read(file, 2);
  const size_t skip = _toml_skip_spaces(file);
  const size_t len = strlen(file->rawData[LINE_OF_SET(file->line)]);
  if (file->rawData[LINE_OF_SET(file->line)][skip] == COMMENT) {
    _toml_add_to_read(file, 1, 1);
    return _toml_read_line(file, size);
  }
  if (size)
    *size = len;
  _toml_set_readLine_len(file, len);
  return file->rawData[LINE_OF_SET(file->line)];
}

// dose not alloc memorry
const char* toml_readline(tomlFile* file, ssize_t* size) {
  if (size)
    *size = -1;
  _toml_add_to_read(file, 1, 1);
  if (!file || file->line >= file->totalLine)
    return NULL;
  return _toml_read_line(file, size);
}


static int _toml_get_name(tomlFile* file) {
  const ssize_t space = _toml_skip_spaces(file);
  if (space == -1)
    return 1;
  int table = 0;
  _toml_add_to_read(file, 2, space);
  while (table < 3 && file->cursor < (size_t)file->currentLen) {
    _toml_add_to_read(file, 2, 1);
  }
  _toml_print_l(file);
  return 0;
}

int toml_is_file_valid(tomlFile* file) {
  int error = 0; // total error
  if (!file)
    return 1;
  if (file->fileSize > MAX_FILE_SIZE || file->totalLine < 1)
    return 2;
  toml_zero_read(file);
  if (!file->keysList) {
    
  }
  //todo att calloc for keysList
  ssize_t lineLen = 0;
  const char* line = "";
  while (line) {
    line = toml_readline(file, &lineLen);
    const int invalid = _toml_get_name(file);
    if (line) {
      error += invalid;
      if (invalid)
        _toml_print_error_parsing(file, file->line);
    }
  }
  return error;
}
