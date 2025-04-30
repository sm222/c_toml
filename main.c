
#include "src/c_toml.h"

int main(int ac, char** av) {
  printf("\n%s\n\n", toml_version());
  for (int i = 1; i < ac; i++) {
    tomlFile* file = toml_init(av[i]);
    toml_info(file);
    const char* l = "";
    while (l) {
      ssize_t error = 0;
      l = toml_readline(file, &error);
      if (l)
        printf("%-5.lu| %s\n", error, l);
    }
    int error = toml_is_file_valid(file);
    printf("%d\n", error);
    toml_end(file);
  }
  return 0;
}