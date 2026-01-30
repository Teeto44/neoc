#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include "token.h"

typedef struct Lexer {
    char* src;
    size_t srcLen;
    size_t pos;
    size_t line;
    size_t column;
} Lexer;

/// Takes ownership of the src code, expects it to be null-terminated.
Lexer* create_lexer(char* src);
void destroy_lexer(Lexer* lexer);

Token* get_next_token(Lexer* lexer);

#endif // LEXER_H
