#include <stdlib.h>
#include <string.h>

#include "helpers.h"

char* int_to_binary(int integer) {
        char* binary = malloc(1 * sizeof(char)); 
        binary[0] = '\0';
        while (integer > 0) {
                size_t len = strlen(binary);
                binary = realloc(binary, strlen(binary) + 2);
                memcpy(binary + len, integer % 2 == 0 ? "0" : "1", 2);
                integer /= 2;
        }
        char *reversed = malloc((strlen(binary) + 1) * sizeof(char));
        for (int i = strlen(binary) - 1; i >= 0; i--) {
                reversed[strlen(binary) - i - 1] = binary[i];
        }
        return reversed; 
}

/* Converts a string into binary */
char* string_to_binary(char* s) {
        if (s == NULL) return 0;
        size_t length = strlen(s);
        char *binary = malloc(length * 8 + 1);
        binary[0] = '\0';
        for (size_t i = 0; i < length; i++) {
                char currentChar = s[i];
                for (int ii = 7; ii >= 0; ii--) {
                        strcat(binary, currentChar & (1 << ii) ? "1" : "0");
                }
        }
        return binary;
}
