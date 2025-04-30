# c_toml
C library for parsing TOML language.

base on the [TOML](https://github.com/toml-lang/toml) language

work in progress

example how to use it
```c
#include "src/c_toml.h"

int main(void) {
  tomlFile* file = toml_init("test/test.toml");
  
  toml_info(file);
  if (file) {
    ssize_t size = 0;
    while (size > -1) {
      const char* line = toml_readline(file, &size); // size can be NULL
      // you can also do: if (!line) break ;
      if (line)
        printf("%5.zu : %s\n", size, line);
    }
  }
  toml_end(file);
  return 0;
}
```

