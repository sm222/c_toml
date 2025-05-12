#include "toml_test_key_name.h"

int _toml_test_key(tomlFile file) {
  if (!file)
    return -1;
  const char* line = _toml_current_line(file);
  
}