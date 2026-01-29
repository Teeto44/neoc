#include "lexer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "token.h"

Lexer lexer = {0};

static char peek(size_t offset);
static void advance(void);
static void skip_whitespace(void);

void lexer_open_src(const char* src) {
    if (src == NULL) {
        fprintf(stderr, "Error: Lexer recieved no source code\n");
        exit(EXIT_FAILURE);
    }

    char* srcBuffer = (char*)malloc(strlen(src) + 1);
    if (srcBuffer == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for source buffer\n");
        exit(EXIT_FAILURE);
    }
    strcpy(srcBuffer, src);

    lexer.src = srcBuffer;
    lexer.pos = 0;
    lexer.column = 1;
    lexer.line = 1;
}

void lexer_close_src(void) {
    if (lexer.src != NULL) {
        free(lexer.src);
    }

    lexer.src = NULL;
}

Token* get_next_token(void) {
    if (lexer.src == NULL) {
        fprintf(stderr, "Error: Token requested before opening a file\n");
        exit(EXIT_FAILURE);
    }

    skip_whitespace();

    TokenType type = TOK_INVALID;

    if (peek(0) == '\0') {
        type = TOK_EOF;
    }

    printf("Current character: '%c'\n", peek(0));

    Token* token = (Token*)malloc(sizeof(Token));
    if (token == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for token\n");
        exit(EXIT_FAILURE);
    }

    token->type = type;
    token->ident = NULL;
    token->line = lexer.line;
    token->column = lexer.column;

    advance();

    return token;
}

/* --- Helper Functions --- */

static char peek(size_t offset) {
    return lexer.src[lexer.pos + offset];
}

static void advance(void) {
    if (peek(0) == '\0') {
        return;
    }

    if (peek(0) == '\n') {
        lexer.line++;
        lexer.column = 0;
    }

    lexer.pos++;
    lexer.column++;
}

static void skip_whitespace(void) {
    while (peek(0) == ' ' || peek(0) == '\t' ||
           peek(0) == '\n' || peek(0) == '\r') {
        advance();
    }
}
