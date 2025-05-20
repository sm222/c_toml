#ifndef TOML_NUMERALS
# define TOML_NUMERALS

# include <stdbool.h>
# include <string.h>
# include <ctype.h>
# include <stdio.h>

# define MIN_ 3
# define MAX_HEXA 10

// Hexa
bool _toml_verify_within_hexa_limit(char c);
bool _toml_verify_hexa_value(char* val);

// Octal
bool _toml_verify_within_octal_limit(char c);
bool _toml_verify_octal_value(char* val);

// Float
bool _toml_verify_float_value(char* val, char* allowedSymbols);

#endif
