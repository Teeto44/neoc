#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include "token.h"

typedef struct Lexer {
    char* src;
    size_t pos;
    size_t line;
    size_t column;
} Lexer;

void lexer_open_src(const char* src);
void lexer_close_src(void);

Token* get_next_token(void);

#endif // LEXER_H
