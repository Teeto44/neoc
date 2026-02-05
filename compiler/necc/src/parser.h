#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct Parser {
    Lexer* lexer;
} Parser;

Parser* create_parser(char* src);
void destroy_parser(Parser* parser);

void parse_program(Parser* parser);

#endif // PARSER_H
