#include "../src/parsing/toml_numerals.h"
#include "../src/gnl.h"
#include <stdlib.h>

int main ()
{
	do {
		write(1, "insert hexa\n", 12);
		char* response = get_next_line(0);
		if (!response)
			break;
		size_t l = strlen(response);
		response[l - 1 ] = '\0';
		int retard = _toml_verify_hexa_value(response) + 48;
		write(1, &retard, 1);
		write(1, "\n", 1);
		free(response);
	} while (1);
}
