#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include "token.h"

typedef struct Lexer {
    char* src;
    size_t line;
    size_t column;
} Lexer;

Lexer lexer;

void lexer_open_file(const char *filepath);
void lexer_close_file(void);

Token* get_next_token(void);

#endif // LEXER_H
