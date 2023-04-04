#include "codec.h"

void encode(char *input, char *output) {
    int i = 0;
    while (input[i] != '\0') {
        output[i] = input[i] + 3;
        i++;
    }
    output[i] = '\0';
}

void decode(char *input, char *output) {
    int i = 0;
    while (input[i] != '\0') {
        output[i] = input[i] - 3;
        i++;
    }
    output[i] = '\0';
}
