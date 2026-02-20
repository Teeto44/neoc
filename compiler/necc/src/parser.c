#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

Parser* create_parser(char* src) {
    Parser* parser = malloc(sizeof(Parser));
    if (parser == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for parser\n");
        return NULL;
    }

    Lexer* lexer = create_lexer(src);
    if (lexer == NULL) {
        free(parser);
        return NULL;
    }

    parser->lexer = lexer;

    return parser;
}

void destroy_parser(Parser* parser) {
    if (parser == NULL) {
        return;
    }

    if (parser->lexer != NULL) {
        destroy_lexer(parser->lexer);
    }

    free(parser);
}

ASTNode* parse_program(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Error: Parser is NULL\n");
        return NULL;
    }

    if (parser->lexer == NULL) {
        fprintf(stderr, "Error: Parser is uninitialized\n");
        return NULL;
    }

    // Parse program here

    return NULL;
}
