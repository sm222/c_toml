#include "toml_test_key_name.h"
#include <string.h>


static bool _toml_test_valid_char(const char c) {
  for (size_t i = 0; i < VALID_VAR_NAME_SECTION; i++) {
    for (char v = VALID_VAR_NAME[i * 2]; v < VALID_VAR_NAME[i * 2 + 1] + 1; v++) {
      if (c == v)
        return true;
    }
  }
  return false;
}

static int _toml_test_key_name(tomlFile file, char* buff, size_t* buffIndex) {
  char c = '\0';
  do {
    const int quotation = toml_quotation(file);
    if (quotation < 0)
      return -1;
    _toml_skip_spaces(file);
    c = _toml_current_char(file);
    if (c == '.')
      return 1;
    while(_toml_test_valid_char(c)) {
      buff[*buffIndex] = c;
      (*buffIndex)++;
      _toml_add_to_read(file, 2, 1);
    }
  } while (1);
  return 0;
}

int _toml_test_key(tomlFile file) {
  if (!file)
    return -1;
  _toml_zero_read(file, 2);
  if (_toml_skip_spaces(file) != 0)
    return 1;
  char   buff[MAX_LINE_LEN + 1];
  size_t buffIndex = 0;
  int    table = 0;
  memset(buff, 0, MAX_LINE_LEN + 1);
  char c = '\0';
  do {
    c = _toml_current_char(file);
    if (c == TABLE_SIMBOLE[TABLE_OPEN])
      table++;
    buff[buffIndex++] = c;
    _toml_add_to_read(file, 2, 1);
  } while (table < 2 && c == TABLE_SIMBOLE[TABLE_OPEN]);
  if (table > 2)
    return  1;
  if (table == 0) {
    return _toml_test_key_name(file, buff, &buffIndex);
  }
  return 0;
}