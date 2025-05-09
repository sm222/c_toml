
#include "src/c_toml.h"


int main(int ac, char** av) {
  printf("\n%s\n%s \n", toml_version(), KEY_VALUE(10));
  for (int i = 1; i < ac; i++) {
    tomlFile* file = toml_init(av[i]);
    toml_info(file);
    printf("file error %d\n", file->error);
    const char* l = "";
    int line = 0;
    while (l) {
      ssize_t error = 0;
      l = toml_readline(file, &error);
      if (l)
        printf("%-5.d %5.lu| %s\n", ++line ,error, l);
    }
    int error = toml_is_file_valid(file);
    printf("%d\n", error);
    toml_end(file);
  }
  return 0;
}