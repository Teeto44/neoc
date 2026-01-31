#include "lexer.h"
#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "token.h"

static char peek(Lexer* lexer, size_t offset);
static void advance(Lexer* lexer);
static void skip_whitespace(Lexer* lexer);
static char* read_identifier(Lexer* lexer);
static char* read_number(Lexer* lexer);
static TokenType get_ident_type(const char* ident);
static TokenType get_num_type(const char* num);

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
    char curChar = peek(lexer, 0);
    switch (curChar) {
        // EOF
        case '\0':
            token = create_token(TOK_EOF, NULL, lexer->line, lexer->column);
            break;

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
                fprintf(stderr, "Lexer Error: Expected '&' got '%c'\n",
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
                fprintf(stderr, "Lexer Error: Expected '|' got '%c'\n",
                    peek(lexer, 0));
                token = create_token(TOK_INVALID, NULL, lexer->line,
                    lexer->column);
            }
            break;

        // Punctuation
        case '(':
            token = create_token(TOK_LPAREN, NULL, lexer->line,
                lexer->column);
            break;
        case ')':
            token = create_token(TOK_RPAREN, NULL, lexer->line,
                lexer->column);
            break;
        case '{':
            token = create_token(TOK_LBRACE, NULL, lexer->line,
                lexer->column);
            break;
        case '}':
            token = create_token(TOK_RBRACE, NULL, lexer->line,
                lexer->column);
            break;
        case ',':
            token = create_token(TOK_COMMA, NULL, lexer->line,
                lexer->column);
            break;
        case ';':
            token = create_token(TOK_SEMICOLON, NULL, lexer->line,
                lexer->column);
            break;

        // Character literal
        case '\'':
            advance(lexer);

            if (!isalpha(peek(lexer, 0))) {
                fprintf(stderr, "Lexer Error: Expected character literal"\
                    " after '\n");
                token = create_token(TOK_INVALID, NULL, lexer->line,
                    lexer->column);
                return token;
            }

            char* charLit = malloc(sizeof(char) * 2);
            if (charLit == NULL) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                token = create_token(TOK_INVALID, NULL, lexer->line,
                    lexer->column);
                return token;
            }
            charLit[0] = peek(lexer, 0);
            charLit[1] = '\0';

            advance(lexer);

            if (!(peek(lexer, 0) == '\'')) {
                fprintf(stderr, "Lexer Error: Expected ' after character"\
                    " literal\n");
                free(charLit);
                token = create_token(TOK_INVALID, NULL, lexer->line,
                    lexer->column);
                return token;
            }

            advance(lexer);

            token = create_token(TOK_CHAR_LIT, charLit, lexer->line,
                lexer->column);
            break;

        // Identifiers, keywords, and literals
        default:
            // Identifier
            if (isalpha(curChar) || curChar == '_') {
                char* ident = read_identifier(lexer);
                if (ident == NULL) {
                    fprintf(stderr, "Error: Failed to read identifier\n");
                    return NULL;
                }
                TokenType type = get_ident_type(ident);

                if (!(type == TOK_IDENT || type == TOK_BOOL_LIT)) {
                    free(ident);
                    ident = NULL;
                }

                token = create_token(type, ident, lexer->line,
                    lexer->column);
                return token;
            }
            // Numeric Literals
            else if (isdigit(curChar)) {
                char* num = read_number(lexer);
                if (num == NULL) {
                    fprintf(stderr, "Error: Failed to read number\n");
                    return NULL;
                }
                TokenType type = get_num_type(num);

                token = create_token(type, num, lexer->line,
                    lexer->column);
                return token;
            }
            else {
                fprintf(stderr, "Lexer Error: Unexpected token '%c'\n",
                    curChar);
                token = create_token(TOK_INVALID, NULL, lexer->line,
                    lexer->column);
            }
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
                    fprintf(stderr, "Lexer Error: Unterminated multiline"\
                        " comment\n");
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

static char* read_identifier(Lexer* lexer) {
    size_t startPos = lexer->pos;
    while (isalnum(peek(lexer, 0)) || peek(lexer, 0) == '_') {
        advance(lexer);
    }

    size_t len = lexer->pos - startPos;
    char* ident = malloc(len + 1);
    if (ident == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }
    memcpy(ident, lexer->src + startPos, len);
    ident[len] = '\0';

    return ident;
}

static char* read_number(Lexer* lexer) {
    size_t startPos = lexer->pos;
    bool hasDigits = false;
    bool hasDigitsBeforeDot = false;
    bool hasDigitsAfterDot = false;

    // Read integer part
    while (isdigit(peek(lexer, 0))) {
        hasDigits = true;
        hasDigitsBeforeDot = true;
        advance(lexer);
    }

    // Check for decimal point
    if (peek(lexer, 0) == '.') {
        advance(lexer);

        // Read fractional part
        while (isdigit(peek(lexer, 0))) {
            hasDigits = true;
            hasDigitsAfterDot = true;
            advance(lexer);
        }

        // Check for digits on both sides
        if (!hasDigitsBeforeDot || !hasDigitsAfterDot) {
            fprintf(stderr, "Lexer Error: Float literal must have digits on"\
                " both sides of the decimal point\n");
            return NULL;
        }
    }

    // Must have at least one digit
    if (!hasDigits) {
        fprintf(stderr, "Lexer Error: Numeric literal must have at least"\
            " one digit\n");
        return NULL;
    }

    size_t len = lexer->pos - startPos;
    char* num = malloc(len + 1);
    if (num == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }
    memcpy(num, lexer->src + startPos, len);
    num[len] = '\0';

    return num;
}

static TokenType get_ident_type(const char* ident) {
    // The speed of this could be GREATLY improved by using switch
    // statements however while the syntax is still pretty up in the air
    // keeping it simple cant hurt, makes it easier to edit.

    //Keywords
    if (!strcmp(ident, "fn")) return TOK_FN;
    if (!strcmp(ident, "return")) return TOK_RETURN;
    if (!strcmp(ident, "mut")) return TOK_MUT;
    if (!strcmp(ident, "if")) return TOK_IF;
    if (!strcmp(ident, "else")) return TOK_ELSE;

    // Types
    if (!strcmp(ident, "i8")) return TOK_I8;
    if (!strcmp(ident, "i16")) return TOK_I16;
    if (!strcmp(ident, "i32")) return TOK_I32;
    if (!strcmp(ident, "i64")) return TOK_I64;
    if (!strcmp(ident, "i128")) return TOK_I128;
    if (!strcmp(ident, "u8")) return TOK_U8;
    if (!strcmp(ident, "u16")) return TOK_U16;
    if (!strcmp(ident, "u32")) return TOK_U32;
    if (!strcmp(ident, "u64")) return TOK_U64;
    if (!strcmp(ident, "u128")) return TOK_U128;
    if (!strcmp(ident, "f32")) return TOK_F32;
    if (!strcmp(ident, "f64")) return TOK_F64;
    if (!strcmp(ident, "bool")) return TOK_BOOL;
    if (!strcmp(ident, "char")) return TOK_CHAR;

    // Bool literals
    if (!strcmp(ident, "true")) return TOK_BOOL_LIT;
    if (!strcmp(ident, "false")) return TOK_BOOL_LIT;

    return TOK_IDENT;
}

static TokenType get_num_type(const char* num) {
    // Check for leading zero
    if (num[0] == '0' && isdigit(num[1])) {
        fprintf(stderr, "Lexer Error: Leading zero in numeric literal\n");
        return TOK_INVALID;
    }

    // Check for float
    if (strchr(num, '.') != NULL) {
        return TOK_FLOAT_LIT;
    }

    return TOK_INT_LIT;
}
