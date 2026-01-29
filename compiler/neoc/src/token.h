#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>
#include <stdlib.h>

typedef enum TokenType {
    // Default
    TOK_INVALID,

    // Keywords
    TOK_FN,
    TOK_RETURN,
    TOK_MUT,
    TOK_IF,
    TOK_ELSE,

    // Types
    TOK_I8,
    TOK_I16,
    TOK_I32,
    TOK_I64,
    TOK_I128,
    TOK_U8,
    TOK_U16,
    TOK_U32,
    TOK_U64,
    TOK_U128,
    TOK_F32,
    TOK_F64,
    TOK_BOOL,
    TOK_CHAR,

    // Punctuation
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACKET,
    TOK_RBRACKET,
    TOK_COMMA,
    TOK_SEMICOLON,

    // Arithmetic operators
    TOK_ADD,
    TOK_SUB,
    TOK_MUL,
    TOK_DIV,
    TOK_MOD,
    TOK_INCREMENT,
    TOK_DECREMENT,

    // Comparison operators
    TOK_EQ,
    TOK_NEQ,
    TOK_LT,
    TOK_LTE,
    TOK_GT,
    TOK_GTE,

    // Logical operators
    TOK_AND,
    TOK_OR,
    TOK_NOT,

    // Assignment operators
    TOK_ASSIGN,
    TOK_PLUS_ASSIGN,
    TOK_MINUS_ASSIGN,
    TOK_MUL_ASSIGN,
    TOK_DIV_ASSIGN,
    TOK_MOD_ASSIGN,

    // Literals
    TOK_INT_LIT,
    TOK_FLOAT_LIT,
    TOK_BOOL_LIT,
    TOK_CHAR_LIT,

    // Misc
    TOK_IDENT,
    TOK_EOF,
} TokenType;

typedef struct Token {
    TokenType type;
    char* ident;
    size_t line;
    size_t column;
} Token;

static inline void free_token(Token *token) {
    if (token->ident != NULL) {
        free(token->ident);
        token->ident = NULL;
    }

    free(token);
}

#endif // TOKEN_H
