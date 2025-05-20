#include "toml_numerals.h"

/*
 *  Hexadecimal
 */
bool _toml_verify_within_hexa_limit(char c) {
	bool retVal = false;
	if (isdigit(c))
		retVal = true;
	if (isalpha(c)) {
		char current = tolower(c);
		if (current >= 'a' && current <= 'f')
			retVal = true;
	}
	return (retVal);
}

bool _toml_verify_hexa_value(char* val) {
	if (!val)
			return (false);
	const size_t lenVal = strlen(val);
	if (lenVal < MIN_ || lenVal > MAX_HEXA)
		return (false);
	if (strncmp(val, "0x", 2) != 0)
		return (false);
	int i = 2;
	while (val[i]) {
		if (!_toml_verify_within_hexa_limit(val[i]))
			return (false);
		i++;
	}
	return (true);
}

/*
 *  Octal
 */
bool _toml_verify_within_octal_limit(char c) {
	return (c >= '0' && c <= '7');
}

bool _toml_verify_octal_value(char* val) {
	if (!val)
		return (false);
	const size_t lenVal = strlen(val);
	if (lenVal < MIN_ || lenVal > 11)
		return (false);
	if (strncmp(val, "0o", 2) != 0)
		return (false);
	int i = 2;
	while (val[i]) {
		if (!_toml_verify_within_octal_limit(val[i]))
			return (false);
		i++;
	}
	return (true);
}

/*
 *  Float, Exponents & Fractionals
 */
bool _toml_verify_float_value(char* val, char* allowedSymbols) {
	if (!val)
		return (false);
	int i = 0;
	while (val[i]) {
		if (!isdigit(c) && !strchr(allowedSymbols, c))
			return (false);
		i++;
	}
	return (true);
}


