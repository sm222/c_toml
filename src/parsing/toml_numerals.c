#include "toml_numerals.h"

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
	if (lenVal < 3 || lenVal > 10)
		return (false);
	if (val[0] != '0' || val[1] != 'x')
		return (false);
	int i = 2;
	while (val[i]) {
		if (!_toml_verify_within_hexa_limit(val[i]))
			return (false);
		i++;
	}
	return (true);
}
	
