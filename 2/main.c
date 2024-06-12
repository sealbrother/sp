#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"

int readText(char *fileName, char *text, int size) {
    FILE *file = fopen(fileName, "r");
    if (!file) {
        perror("Failed to open file");
        return -1;
    }
    int len = fread(text, 1, size - 1, file);
    if (ferror(file)) {
        perror("Failed to read file");
        fclose(file);
        return -1;
    }
    text[len] = '\0';
    fclose(file);
    return len;
}

void dump(char *strTable[], int top) {
    printf("========== dump ==============\n");
    for (int i = 0; i < top; i++) {
        printf("%d:%s\n", i, strTable[i]);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
        return 1;
    }

    if (readText(argv[1], code, TMAX) == -1) {
        return 1;
    }

    puts(code);
    lex(code);
    dump(tokens, tokenTop);
    parse();
    return 0;
}