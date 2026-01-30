#include "lexer.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "token.h"

static char peek(Lexer* lexer, size_t offset);
static void advance(Lexer* lexer);
static void skip_whitespace(Lexer* lexer);

Lexer* create_lexer(char* src) {
    if (src == NULL) {
        fprintf(stderr, "Error: Lexer received no source code\n");
        return NULL;
    }

    Lexer* lexer = malloc(sizeof(Lexer));
    if (lexer == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for lexer\n");
        return NULL;
    }

    lexer->src = src;
    lexer->srcLen = strlen(src);
    lexer->pos = 0;
    lexer->column = 1;
    lexer->line = 1;

    return lexer;
}

void destroy_lexer(Lexer* lexer) {
    if (lexer == NULL) {
        return;
    }

    if (lexer->src != NULL) {
        free(lexer->src);
        lexer->src = NULL;
    }

    free(lexer);
}

Token* get_next_token(Lexer* lexer) {
    if (lexer == NULL) {
        fprintf(stderr, "Error: Token requested from null lexer\n");
        return NULL;
    }

    if (lexer->src == NULL) {
        fprintf(stderr, "Error: Token requested before opening a file\n");
        return NULL;
    }

    skip_whitespace(lexer);

    Token* token = NULL;
    switch (peek(lexer, 0)) {
        // Operators
        case '+':
            if (peek(lexer, 1) == '=') {
                advance(lexer);
                token = create_token(TOK_PLUS_ASSIGN, NULL, lexer->line,
                    lexer->column);
            } else if (peek(lexer, 1) == '+') {
                advance(lexer);
                token = create_token(TOK_INCREMENT, NULL, lexer->line,
                    lexer->column);
            } else {
                token = create_token(TOK_ADD, NULL, lexer->line,
                    lexer->column);
            }
            break;
        case '-':
            if (peek(lexer, 1) == '=') {
                advance(lexer);
                token = create_token(TOK_MINUS_ASSIGN, NULL, lexer->line,
                    lexer->column);
            } else if (peek(lexer, 1) == '-') {
                advance(lexer);
                token = create_token(TOK_DECREMENT, NULL, lexer->line,
                    lexer->column);
            } else {
                token = create_token(TOK_SUB, NULL, lexer->line,
                    lexer->column);
            }
            break;
        case '*':
            if (peek(lexer, 1) == '=') {
                advance(lexer);
                token = create_token(TOK_MUL_ASSIGN, NULL, lexer->line,
                    lexer->column);
            } else {
                token = create_token(TOK_MUL, NULL, lexer->line,
                    lexer->column);
            }
            break;
        case '/':
            if (peek(lexer, 1) == '=') {
                advance(lexer);
                token = create_token(TOK_DIV_ASSIGN, NULL, lexer->line,
                    lexer->column);
            } else {
                token = create_token(TOK_DIV, NULL, lexer->line,
                    lexer->column);
            }
            break;
        case '%':
            if (peek(lexer, 1) == '=') {
                advance(lexer);
                token = create_token(TOK_MOD_ASSIGN, NULL, lexer->line,
                    lexer->column);
            } else {
                token = create_token(TOK_MOD, NULL, lexer->line,
                    lexer->column);
            }
            break;
        case '=':
            if (peek(lexer, 1) == '=') {
                advance(lexer);
                token = create_token(TOK_EQ, NULL, lexer->line, lexer->column);
            } else {
                token = create_token(TOK_ASSIGN, NULL, lexer->line,
                    lexer->column);
            }
            break;
        case '!':
            if (peek(lexer, 1) == '=') {
                advance(lexer);
                token = create_token(TOK_NEQ, NULL, lexer->line,
                    lexer->column);
            } else {
                token = create_token(TOK_NOT, NULL, lexer->line,
                    lexer->column);
            }
            break;
        case '<':
            if (peek(lexer, 1) == '=') {
                advance(lexer);
                token = create_token(TOK_LTE, NULL, lexer->line,
                    lexer->column);
            } else {
                token = create_token(TOK_LT, NULL, lexer->line, lexer->column);
            }
            break;
        case '>':
            if (peek(lexer, 1) == '=') {
                advance(lexer);
                token = create_token(TOK_GTE, NULL, lexer->line,
                    lexer->column);
            } else {
                token = create_token(TOK_GT, NULL, lexer->line,
                    lexer->column);
            }
            break;
        case '&':
            if (peek(lexer, 1) == '&') {
                advance(lexer);
                token = create_token(TOK_AND, NULL, lexer->line,
                    lexer->column);
            } else {
                fprintf(stderr, "Expected character '&', got '%c'\n",
                    peek(lexer, 0));
                token = create_token(TOK_INVALID, NULL, lexer->line,
                    lexer->column);
            }
            break;
        case '|':
            if (peek(lexer, 1) == '|') {
                advance(lexer);
                token = create_token(TOK_OR, NULL, lexer->line,
                    lexer->column);
            } else {
                fprintf(stderr, "Expected character '|', got '%c'\n",
                    peek(lexer, 0));
                token = create_token(TOK_INVALID, NULL, lexer->line,
                    lexer->column);
            }
            break;
        case '\0':
            token = create_token(TOK_EOF, NULL, lexer->line, lexer->column);
            break;
        default:
            token = create_token(TOK_INVALID, NULL, lexer->line,
                lexer->column);
            break;
    }

    advance(lexer);

    return token;
}

/* --- Helper Functions --- */

static char peek(Lexer* lexer, size_t offset) {
    size_t newPos = lexer->pos + offset;
    if (newPos >= lexer->srcLen) {
        return '\0';
    }

    return lexer->src[newPos];
}

static void advance(Lexer* lexer) {
    char curChar = peek(lexer, 0);
    if (curChar == '\0') {
        return;
    }

    lexer->pos++;
    lexer->column++;

    if (curChar == '\n') {
        lexer->line++;
        lexer->column = 1;
    }
}

static void skip_whitespace(Lexer* lexer) {
    while (true) {
        char curChar = peek(lexer, 0);

        // Whitespace characters
        if (curChar == ' ' || curChar == '\t' || curChar == '\n' ||
            curChar == '\r') {
            advance(lexer);
            continue;
        }
        // Single line comments
        else if (curChar == '/' && peek(lexer, 1) == '/') {
            advance(lexer);
            advance(lexer);

            while (peek(lexer, 0) != '\n' && peek(lexer, 0) != '\0') {
                advance(lexer);
            }

            continue;
        }
        // Multiline comments
        else if (curChar == '/' && peek(lexer, 1) == '*') {
            advance(lexer);
            advance(lexer);

            while (true) {
                if (peek(lexer, 0) == '\0') {
                    fprintf(stderr, "Error: Unterminated multiline comment\n");
                    break;
                }

                if (peek(lexer, 0) == '*' && peek(lexer, 1) == '/') {
                    break;
                }

                advance(lexer);
            }

            advance(lexer);
            advance(lexer);

            continue;
        }
        break;
    }
}
