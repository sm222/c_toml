
#include "c_toml.h"
#include "toml_utils.h"
#include "toml_types.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "gnl.h"


const char*    toml_version(void) {
  static char s[31];
  #ifdef VERSION
  snprintf(s, 30, "c_toml version:%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
  #else
  snprintf(s, 20, "c_toml version: UNDEFINE");
  #endif
  return s;
}

int toml_zero_read(tomlFile* file) {
  if (!file)
    return 1;
  file->line = 0;
  file->cursor = 0;
  return 0;
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
  const int size = 1000;
  char      str[size];
  snprintf(str, size -1,"name: %s\n\tpath: %s\n"  \
    "\tsize: %zu\n\ttotal line: %zu\n\terror %d\n"\
    "line: %zu\ncursor: %zu\n", file->fileName,   \
    file->filePath, file->fileSize, file->totalLine, file->error, file->line, file->cursor);
  write(1, str, strlen(str));
}

// dose not alloc memorry
const char* toml_readline(tomlFile* file, ssize_t* size) {
  if (size)
    *size = -1;
  if (!file)
    return NULL;
  if (file->line >= file->totalLine) {
    return NULL;
  }
  file->cursor = 0;
  const size_t skip = _toml_skip_spaces(file);
  const size_t len = strlen(file->rawData[file->line]);
  if (file->rawData[file->line][skip] == COMMENT) {
    file->line++;
    return toml_readline(file, size);
  }
  if (size)
    *size = len;
  return file->rawData[file->line++];
}


static ssize_t _toml_look_for_valid_name(const char* name, const size_t end) {
  int test = 0;
  for (int i = 0; i < VALID_VAR_NAME_SECSION; i++) {
    for (int j = VALID_VAR_NAME[i * 2]; j < VALID_VAR_NAME[i * 2 + 1] + 1; j++) {
      if (*name == j) {test = j;}
    }
  }
  if (!test)
    return -1;
  size_t i = 1;
  for (; i < end; i++) {
    if (name[i] == TABLE_SIMBOLE[TABLE_CLOSE]) {
        break ;
      }
    if (name[i] == COMMENT) {
      return -1;
    }
  }
  return i;
}


static int _toml_look_type(const char* line, const size_t end, int *type) {
  size_t i = 0;
  int error = 1;
  int table = 0;
  *type = -1;
  while (line[i] == TABLE_SIMBOLE[TABLE_OPEN]) {
    if (table > 2) {
      return 1;
    }
    table++;
    i++;
  }
  const ssize_t parseName = _toml_look_for_valid_name(line + table, end - table);
  if (table < 3 && parseName != -1) {
    *type = table ? (Table + table - 1) : Field;
    i += parseName;
    while (i < end) {
      if (line[i] == TABLE_SIMBOLE[TABLE_CLOSE])
        table--;
      else
        break;
      i++;
    }
    if (table == 0)
      error = 0;
  }
  return error;
}

int toml_get_value(const char* line, const size_t lineLen) {
  if (!line) {
    return 1;
  }
  int type;
  size_t i = 0;
  for (; i < lineLen; i++) {
    if (strchr(VALID_SPACE, line[i]) == NULL)
    break ;
  }
  if (i == lineLen)
    return 1;
  if (_toml_look_type(line + i, lineLen - i, &type))
    return 1;
  return 0;
}

int toml_is_file_valid(tomlFile* file) {
  int error = 0; // total error
  if (!file)
    return 1;
  if (file->fileSize > MAX_FILE_SIZE || file->totalLine < 1)
    return 2;
  toml_zero_read(file);
  ssize_t lineLen = 0;
  for (size_t i = 0; i < file->totalLine - 1; i++) {
    const char* line = toml_readline(file, &lineLen);
    const int valid = toml_get_value(line, lineLen);
    if (line) {
      error += valid;
      _toml_print_error_parsing(file);
    }
  }
  return error;
}
