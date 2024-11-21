#include <stdio.h>
#include <stdlib.h> // heap
#include <string.h>

int main(int argc, char *argv[])
{
    int max_string_length;
    char fmt[6];
    char *input;

    if (argc != 2) {
        printf("Usage: %s <max string length 1..256>\n", argv[0]);
        return -1;
    }

    if (1 != sscanf(argv[1], "%d", &max_string_length)) {
        printf("Error scanning integer for (%s).\n", argv[1]);
        return -1;
    }

    // We need at least 4 characters for 'quit'
    if (max_string_length < 4 || max_string_length > 256) {
        printf("Max string length is out of bounds (<4 or >256): %d\n", max_string_length);
        return -1;
    }

    // scanf/sscanf/fscanf do not have the asterisk * specifier like printf to dynamically
    // limit a specifier. This means we have to do it manually by generating the format string.
    snprintf(fmt, sizeof(fmt), "%%%ds", max_string_length);
    printf("%ds\n", max_string_length);

    // The specifier like %16s limits the number of characters, so we need a spot for
    // the NULL terminator, so we add 1 to the number of characters.
    input = malloc(max_string_length + 1);
    printf("Enter values ('quit' to quit):\n");
    while (1 == scanf(fmt, input)) {
        if (!strcmp(input, "quit")) {
            break;
        }
        printf("Got: '%s'.\n", input);
    }

    free(input);
}
