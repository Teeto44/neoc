#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct Parser {
    /// The lexer used by the parser.
    Lexer* lexer;
} Parser;

/// Creates a parser from the given source code. Takes ownership of the
/// src and expects it to be null-terminated. Returns a pointer to the
/// newly created parser, or NULL if src is not valid or memory
/// allocation fails.
Parser* create_parser(char* src);
/// Frees the memory allocated for the parser, including the owned
/// lexer and its source string. Safely handles NULL.
void destroy_parser(Parser* parser);

/// Parses the source code owned by the parser. Returns a pointer to the
/// root node of the abstract syntax tree, or NULL if parsing fails.
ASTNode* parse_program(Parser* parser);

#endif // PARSER_H
