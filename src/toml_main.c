
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

int toml_zero_read(tomlFile file) {
  return _toml_zero_read(file, 3);
}

/// @brief 
/// @param filePath 
/// @return only time it return null is if calloc fail
tomlFile* toml_init(const char* filePath) {
  return (tomlFile*)_toml_init_data_(filePath);
}


int toml_end(tomlFile file) {
  if (!file)
    return 1;
  _toml_free_file(file);
  return 0;
}


void toml_info(const tomlFile file) {
  _toml_info(file);
}

int toml_get_error(const tomlFile file) {
  return _toml_get_error(file);
}

// dose not alloc memorry
const char* toml_readline(tomlFile file, ssize_t* size) {
  if (size)
    *size = -1;
  _toml_add_to_read(file, 1, 1);
  return _toml_read_line(file, size);
}

int toml_is_file_valid(tomlFile* file) {
  int error = 0; // total error
  if (!file)
    return 1;
  const ssize_t fileSize = _toml_get_file_byte_size(file);
  if (fileSize == -1 || fileSize > MAX_FILE_SIZE || _toml_get_file_line_number(file) < 1)
    return 2;
  toml_zero_read(file);
  //todo att calloc for keysList
  ssize_t lineLen = 0;
  const char* line = "";
  while (line) {
    line = toml_readline(file, &lineLen);
    const int invalid = _toml_get_name(file);
    if (line) {
      error += invalid;
      if (invalid)
        _toml_print_error_parsing(file);
    }
  }
  return error;
}
