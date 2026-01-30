#include "lexer.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
    lexer.srcLen = strlen(srcBuffer);
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

    Token* token = NULL;
    switch (peek(0)) {
        // Operators
        case '+':
            if (peek(1) == '=') {
                advance();
                token = create_token(TOK_PLUS_ASSIGN, NULL, lexer.line,
                    lexer.column);
            } else if (peek(1) == '+') {
                advance();
                token = create_token(TOK_INCREMENT, NULL, lexer.line,
                    lexer.column);
            } else {
                token = create_token(TOK_ADD, NULL, lexer.line, lexer.column);
            }
            break;
        case '-':
            if (peek(1) == '=') {
                advance();
                token = create_token(TOK_MINUS_ASSIGN, NULL, lexer.line,
                    lexer.column);
            } else if (peek(1) == '-') {
                advance();
                token = create_token(TOK_DECREMENT, NULL, lexer.line,
                    lexer.column);
            } else {
                token = create_token(TOK_SUB, NULL, lexer.line, lexer.column);
            }
            break;
        case '*':
            if (peek(1) == '=') {
                advance();
                token = create_token(TOK_MUL_ASSIGN, NULL, lexer.line,
                    lexer.column);
            } else {
                token = create_token(TOK_MUL, NULL, lexer.line, lexer.column);
            }
            break;
        case '/':
            if (peek(1) == '=') {
                advance();
                token = create_token(TOK_DIV_ASSIGN, NULL, lexer.line,
                    lexer.column);
            } else {
                token = create_token(TOK_DIV, NULL, lexer.line, lexer.column);
            }
            break;
        case '%':
            if (peek(1) == '=') {
                advance();
                token = create_token(TOK_MOD_ASSIGN, NULL, lexer.line,
                    lexer.column);
            } else {
                token = create_token(TOK_MOD, NULL, lexer.line, lexer.column);
            }
            break;
        case '=':
            if (peek(1) == '=') {
                advance();
                token = create_token(TOK_EQ, NULL, lexer.line, lexer.column);
            } else {
                token = create_token(TOK_ASSIGN, NULL, lexer.line,
                    lexer.column);
            }
            break;
        case '!':
            if (peek(1) == '=') {
                advance();
                token = create_token(TOK_NEQ, NULL, lexer.line, lexer.column);
            } else {
                token = create_token(TOK_NOT, NULL, lexer.line, lexer.column);
            }
            break;
        case '<':
            if (peek(1) == '=') {
                advance();
                token = create_token(TOK_LTE, NULL, lexer.line, lexer.column);
            } else {
                token = create_token(TOK_LT, NULL, lexer.line, lexer.column);
            }
            break;
        case '>':
            if (peek(1) == '=') {
                advance();
                token = create_token(TOK_GTE, NULL, lexer.line, lexer.column);
            } else {
                token = create_token(TOK_GT, NULL, lexer.line, lexer.column);
            }
            break;
        case '&':
            if (peek(1) == '&') {
                advance();
                token = create_token(TOK_AND, NULL, lexer.line, lexer.column);
            } else {
                fprintf(stderr, "Expected character '&', got '%c'", peek(0));
                token = create_token(TOK_INVALID, NULL, lexer.line,
                    lexer.column);
            }
            break;
        case '|':
            if (peek(1) == '|') {
                advance();
                token = create_token(TOK_OR, NULL, lexer.line,
                    lexer.column);
            } else {
                fprintf(stderr, "Expected character '|', got '%c'", peek(0));
                token = create_token(TOK_INVALID, NULL, lexer.line,
                    lexer.column);
            }
            break;
        case '\0':
            token = create_token(TOK_EOF, NULL, lexer.line, lexer.column);
            break;
        default:
            token = create_token(TOK_INVALID, NULL, lexer.line, lexer.column);
            break;
    }

    advance();

    return token;
}

/* --- Helper Functions --- */

static char peek(size_t offset) {
    size_t newPos = lexer.pos + offset;
    if (newPos >= lexer.srcLen) {
        return '\0';
    }

    return lexer.src[newPos];
}

static void advance(void) {
    char curChar = peek(0);
    if (curChar == '\0') {
        return;
    }

    if (curChar == '\n') {
        lexer.line++;
        lexer.column = 0;
    }

    lexer.pos++;
    lexer.column++;
}

static void skip_whitespace(void) {
    while (true) {
        char curChar = peek(0);

        // Whitespace characters
        if (curChar == ' ' || curChar == '\t' || curChar == '\n' ||
            curChar == '\r') {
            advance();
            continue;
        }
        // Single line comments
        else if (curChar == '/' && peek(1) == '/') {
            advance();
            advance();

            while (peek(0) != '\n' && peek(0) != '\0') {
                advance();
            }

            continue;
        }
        // Multiline comments
        else if (curChar == '/' && peek(1) == '*') {
            advance();
            advance();

            while (true) {
                if (peek(0) == '\0') {
                    fprintf(stderr, "Error: Unterminated multiline comment\n");
                    exit(EXIT_FAILURE);
                }

                if (peek(0) == '*' && peek(1) == '/') {
                    break;
                }

                advance();
            }

            advance();
            advance();

            continue;
        }
        break;
    }
}
