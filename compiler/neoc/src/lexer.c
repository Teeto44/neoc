#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

void lexer_open_file(const char *filepath) {
    if (lexer.src != NULL) {
        lexer_close_file();
    }

    if (filepath == NULL) {
        fprintf(stderr, "Error: File path is NULL\n");
        exit(EXIT_FAILURE);
    }

    FILE *srcFile = fopen(filepath, "rb");
    if (srcFile == NULL) {
        fprintf(stderr, "Error: Failed to open file '%s'\n", filepath);
        exit(EXIT_FAILURE);
    }

    fseek(srcFile, 0, SEEK_END);
    size_t fileSize = ftell(srcFile);
    fseek(srcFile, 0, SEEK_SET);

    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for file buffer\n");
        fclose(srcFile);
        exit(EXIT_FAILURE);
    }

    size_t bytesRead = fread(buffer, 1, fileSize, srcFile);
    buffer[bytesRead] = '\0';

    lexer.src = buffer;
    lexer.column = 1;
    lexer.line = 1;

    fclose(srcFile);
}

void lexer_close_file(void) {
    if (lexer.src != NULL) {
        free(lexer.src);
    }

    lexer.src = NULL;
}

Token* get_next_token(void);
