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
static char* read_number(Lexer* lexer, size_t startLine, size_t startColumn);
static TokenType get_ident_type(const char* ident);
static TokenType get_num_type(const char* num, size_t startLine,
    size_t startColumn);

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

    size_t startLine = lexer->line;
    size_t startColumn = lexer->column;

    Token* token = NULL;
    char curChar = peek(lexer, 0);

    switch (curChar) {
        // EOF
        case '\0':
            token = create_token(TOK_EOF, NULL, startLine, startColumn);
            advance(lexer);
            break;

        // Operators
        case '+':
            if (peek(lexer, 1) == '=') {
                token = create_token(TOK_PLUS_ASSIGN, NULL, startLine,
                    startColumn);
                advance(lexer);
                advance(lexer);
            } else if (peek(lexer, 1) == '+') {
                token = create_token(TOK_INCREMENT, NULL, startLine,
                    startColumn);
                advance(lexer);
                advance(lexer);
            } else {
                token = create_token(TOK_ADD, NULL, startLine, startColumn);
                advance(lexer);
            }
            break;
        case '-':
            if (peek(lexer, 1) == '=') {
                token = create_token(TOK_MINUS_ASSIGN, NULL, startLine,
                    startColumn);
                advance(lexer);
                advance(lexer);
            } else if (peek(lexer, 1) == '-') {
                token = create_token(TOK_DECREMENT, NULL, startLine,
                    startColumn);
                advance(lexer);
                advance(lexer);
            } else {
                token = create_token(TOK_SUB, NULL, startLine, startColumn);
                advance(lexer);
            }
            break;
        case '*':
            if (peek(lexer, 1) == '=') {
                token = create_token(TOK_MUL_ASSIGN, NULL, startLine,
                    startColumn);
                advance(lexer);
                advance(lexer);
            } else {
                token = create_token(TOK_MUL, NULL, startLine, startColumn);
                advance(lexer);
            }
            break;
        case '/':
            if (peek(lexer, 1) == '=') {
                token = create_token(TOK_DIV_ASSIGN, NULL, startLine,
                    startColumn);
                advance(lexer);
                advance(lexer);
            } else {
                token = create_token(TOK_DIV, NULL, startLine, startColumn);
                advance(lexer);
            }
            break;
        case '%':
            if (peek(lexer, 1) == '=') {
                token = create_token(TOK_MOD_ASSIGN, NULL, startLine,
                    startColumn);
                advance(lexer);
                advance(lexer);
            } else {
                token = create_token(TOK_MOD, NULL, startLine, startColumn);
                advance(lexer);
            }
            break;
        case '=':
            if (peek(lexer, 1) == '=') {
                token = create_token(TOK_EQ, NULL, startLine, startColumn);
                advance(lexer);
                advance(lexer);
            } else {
                token = create_token(TOK_ASSIGN, NULL, startLine, startColumn);
                advance(lexer);
            }
            break;
        case '!':
            if (peek(lexer, 1) == '=') {
                token = create_token(TOK_NEQ, NULL, startLine, startColumn);
                advance(lexer);
                advance(lexer);
            } else {
                token = create_token(TOK_NOT, NULL, startLine, startColumn);
                advance(lexer);
            }
            break;
        case '<':
            if (peek(lexer, 1) == '=') {
                token = create_token(TOK_LTE, NULL, startLine, startColumn);
                advance(lexer);
                advance(lexer);
            } else {
                token = create_token(TOK_LT, NULL, startLine, startColumn);
                advance(lexer);
            }
            break;
        case '>':
            if (peek(lexer, 1) == '=') {
                token = create_token(TOK_GTE, NULL, startLine, startColumn);
                advance(lexer);
                advance(lexer);
            } else {
                token = create_token(TOK_GT, NULL, startLine, startColumn);
                advance(lexer);
            }
            break;
        case '&':
            if (peek(lexer, 1) == '&') {
                token = create_token(TOK_AND, NULL, startLine, startColumn);
                advance(lexer);
                advance(lexer);
            } else {
                fprintf(stderr, "Lexer Error [%zu:%zu]: Expected '&' got "\
                    "'%c'\n", startLine, startColumn, peek(lexer, 1));
                token = create_token(TOK_INVALID, NULL, startLine,
                    startColumn);
                advance(lexer);
            }
            break;
        case '|':
            if (peek(lexer, 1) == '|') {
                token = create_token(TOK_OR, NULL, startLine, startColumn);
                advance(lexer);
                advance(lexer);
            } else {
                fprintf(stderr, "Lexer Error [%zu:%zu]: Expected '|' after"\
                    " '|' got '%c'\n", startLine, startColumn, peek(lexer, 1));
                token = create_token(TOK_INVALID, NULL, startLine,
                    startColumn);
                advance(lexer);
            }
            break;

        // Punctuation
        case '(':
            token = create_token(TOK_LPAREN, NULL, startLine, startColumn);
            advance(lexer);
            break;
        case ')':
            token = create_token(TOK_RPAREN, NULL, startLine, startColumn);
            advance(lexer);
            break;
        case '{':
            token = create_token(TOK_LBRACE, NULL, startLine, startColumn);
            advance(lexer);
            break;
        case '}':
            token = create_token(TOK_RBRACE, NULL, startLine, startColumn);
            advance(lexer);
            break;
        case ',':
            token = create_token(TOK_COMMA, NULL, startLine, startColumn);
            advance(lexer);
            break;
        case ';':
            token = create_token(TOK_SEMICOLON, NULL, startLine, startColumn);
            advance(lexer);
            break;

        // Character literal
        case '\'':
            advance(lexer);

            char charLit = peek(lexer, 0);

            if (charLit == '\0') {
                fprintf(stderr, "Lexer Error [%zu:%zu]: Unexpected EOF in"\
                    " character literal\n", startLine, startColumn);
                token = create_token(TOK_INVALID, NULL, startLine,
                    startColumn);
                break;
            } else if (charLit == '\'') {
                fprintf(stderr, "Lexer Error [%zu:%zu]: Empty character"\
                    " literal\n", startLine, startColumn);
                token = create_token(TOK_INVALID, NULL, startLine,
                    startColumn);
                advance(lexer);
                break;
            }
            // Escape sequence
            else if (charLit == '\\') {
                advance(lexer);
                char escapeChar = peek(lexer, 0);

                if (escapeChar == '\0') {
                    fprintf(stderr, "Lexer Error [%zu:%zu]: Unexpected EOF in"\
                        " escape sequence\n", startLine, startColumn);
                    token = create_token(TOK_INVALID, NULL, startLine,
                        startColumn);
                    break;
                }

                switch (escapeChar) {
                    case 'n':  charLit = '\n'; break;
                    case 't':  charLit = '\t'; break;
                    case 'r':  charLit = '\r'; break;
                    case '0':  charLit = '\0'; break;
                    case '\\': charLit = '\\'; break;
                    case '\'': charLit = '\''; break;
                    default:
                        fprintf(stderr, "Lexer Error [%zu:%zu]: Invalid"\
                            " escape sequence '\\%c'\n", startLine,
                            startColumn, escapeChar);
                        token = create_token(TOK_INVALID, NULL, startLine,
                            startColumn);
                        // Skip to closing quote or newline for error recovery
                        while (peek(lexer, 0) != '\'' && peek(lexer, 0) != '\n'
                            && peek(lexer, 0) != '\0') {
                            advance(lexer);
                        }
                        if (peek(lexer, 0) == '\'') {
                            advance(lexer);
                        }
                        break;
                }

                if (token == NULL) {
                    advance(lexer);
                } else {
                    break;
                }
            } else {
                advance(lexer);
            }

            if (peek(lexer, 0) != '\'') {
                fprintf(stderr, "Lexer Error [%zu:%zu]: Expected \"'\" after"\
                    " character literal, got '%c'\n", startLine, startColumn,
                    peek(lexer, 0));
                token = create_token(TOK_INVALID, NULL, startLine,
                    startColumn);

                // Skip until we find a quote or newline
                while (peek(lexer, 0) != '\'' && peek(lexer, 0) != '\n' &&
                    peek(lexer, 0) != '\0') {
                    advance(lexer);
                }
                if (peek(lexer, 0) == '\'') {
                    advance(lexer);
                }
                break;
            }

            advance(lexer);

            char* charIdent = malloc(2);
            if (charIdent == NULL) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                return NULL;
            }
            charIdent[0] = charLit;
            charIdent[1] = '\0';

            token = create_token(TOK_CHAR_LIT, charIdent, startLine,
                startColumn);
            break;

        // Identifiers, keywords, and literals
        default:
            // Identifier
            if (isalpha(curChar) || curChar == '_') {
                char* ident = read_identifier(lexer);
                if (ident == NULL) {
                    return NULL;
                }
                TokenType type = get_ident_type(ident);

                if (!(type == TOK_IDENT || type == TOK_BOOL_LIT)) {
                    free(ident);
                    ident = NULL;
                }

                token = create_token(type, ident, startLine, startColumn);
                return token;
            }
            // Numeric Literals
            else if (isdigit(curChar)) {
                char* num = read_number(lexer, startLine, startColumn);
                if (num == NULL) {
                    return NULL;
                }
                TokenType type = get_num_type(num, startLine, startColumn);

                token = create_token(type, num, startLine, startColumn);
                return token;
            }
            else {
                fprintf(stderr, "Lexer Error [%zu:%zu]: Unexpected token"\
                    " '%c'\n", startLine, startColumn, curChar);
                token = create_token(TOK_INVALID, NULL, startLine,
                    startColumn);
                advance(lexer);
            }
            break;
    }

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
            size_t commentStartLine = lexer->line;
            size_t commentStartCol = lexer->column;

            advance(lexer);
            advance(lexer);

            while (true) {
                if (peek(lexer, 0) == '\0') {
                    fprintf(stderr, "Lexer Error [%zu:%zu]: Unterminated"\
                        " multiline comment\n", commentStartLine,
                        commentStartCol);
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

static char* read_number(Lexer* lexer, size_t startLine, size_t startColumn) {
    size_t startPos = lexer->pos;
    bool hasDigitsAfterDot = false;

    // Read integer part
    while (isdigit(peek(lexer, 0))) {
        advance(lexer);
    }

    // Check for decimal point
    if (peek(lexer, 0) == '.') {
        advance(lexer);

        // Read fractional part
        while (isdigit(peek(lexer, 0))) {
            hasDigitsAfterDot = true;
            advance(lexer);
        }

        // Check for digits after dot
        if (!hasDigitsAfterDot) {
            fprintf(stderr, "Lexer Error [%zu:%zu]: Float literal must have"\
                " digits after decimal point\n", startLine, startColumn);
            return NULL;
        }
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

    // Keywords
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

static TokenType get_num_type(const char* num, size_t startLine,
    size_t startColumn) {
    // Check for leading zero
    if (num[0] == '0' && isdigit(num[1])) {
        fprintf(stderr, "Lexer Error [%zu:%zu]: Leading zero in numeric"\
            " literal\n", startLine, startColumn);
        return TOK_INVALID;
    }

    // Check for float
    if (strchr(num, '.') != NULL) {
        return TOK_FLOAT_LIT;
    }

    return TOK_INT_LIT;
}
