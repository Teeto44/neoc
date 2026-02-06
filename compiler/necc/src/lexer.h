#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include "token.h"

typedef struct Lexer {
    /// Null-terminated copy of source code being lexed.
    char* src;
    /// Length of the source code being lexed.
    size_t srcLen;
    /// Lexer's current position in the source code.
    size_t pos;
    /// Current line number in the source file.
    size_t line;
    /// Current column number in the source file.
    size_t column;
} Lexer;

/// Creates a lexer from the given source code. Takes ownership of the
/// src and expects it to be null-terminated. Returns a pointer to the
/// newly created lexer, or NULL if src is not valid or memory
/// allocation fails.
Lexer* create_lexer(char* src);
/// Frees the memory allocated for the lexer.
void destroy_lexer(Lexer* lexer);
/// Returns a pointer to the next token in the source code. This needs
/// to be freed by the caller. Returns NULL if lexer is not valid or
/// memory allocation fails.
Token* get_next_token(Lexer* lexer);

#endif // LEXER_H
