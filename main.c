
#include "src/c_toml.h"

int main(int ac, char** av) {
  for (int i = 1; i < ac; i++) {
    tomlFile* file =  toml_init(av[i]);
    toml_info(file);
    const char* l = "";
    while (l) {
      ssize_t error = 0;
      l = toml_readline(file, &error);
      if (l)
        printf("%lu|%s\n", error, l);
    }
    toml_end(file);
  }
  return 0;
}