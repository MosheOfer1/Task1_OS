#include <ctype.h>
#include "codec.h"

void encode(char *input, char *output) {
    int i = 0;
    while (input[i] != '\0') {
        if (islower(input[i])) {
            output[i] = toupper(input[i]);
        } else if (isupper(input[i])) {
            output[i] = tolower(input[i]);
        } else {
            output[i] = input[i];
        }
        i++;
    }
    output[i] = '\0';
}

void decode(char *input, char *output) {
    encode(input, output);
}
