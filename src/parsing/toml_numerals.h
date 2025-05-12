#ifndef TOML_NUMERALS
# define TOML_NUMERALS

# include <stdbool.h>
# include <string.h>
# include <ctype.h>
# include <stdio.h>

bool _toml_verify_within_hexa_limit(char c);
bool _toml_verify_hexa_value(char* val);

#endif
