#include "token.h"
#include <stdio.h>
#include <stdlib.h>

Token* create_token(TokenType type, char* ident, size_t line, size_t column) {
    Token* token = malloc(sizeof(Token));
    if (token == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for token\n");
        return NULL;
    }

    token->type = type;
    token->ident = ident;
    token->line = line;
    token->column = column;

    return token;
}

void free_token(Token *token) {
    if (token->ident == NULL) {
    } else {
        free(token->ident);
        token->ident = NULL;
    }

    free(token);
}

const char* token_as_str(TokenType type) {
    switch (type) {
        case TOK_INVALID: return "TOK_INVALID";
        case TOK_FN: return "TOK_FN";
        case TOK_RETURN: return "TOK_RETURN";
        case TOK_MUT: return "TOK_MUT";
        case TOK_IF: return "TOK_IF";
        case TOK_ELSE: return "TOK_ELSE";

        case TOK_I8: return "TOK_I8";
        case TOK_I16: return "TOK_I16";
        case TOK_I32: return "TOK_I32";
        case TOK_I64: return "TOK_I64";
        case TOK_I128: return "TOK_I128";
        case TOK_U8: return "TOK_U8";
        case TOK_U16: return "TOK_U16";
        case TOK_U32: return "TOK_U32";
        case TOK_U64: return "TOK_U64";
        case TOK_U128: return "TOK_U128";
        case TOK_F32: return "TOK_F32";
        case TOK_F64: return "TOK_F64";
        case TOK_BOOL: return "TOK_BOOL";
        case TOK_CHAR: return "TOK_CHAR";

        case TOK_LPAREN: return "TOK_LPAREN";
        case TOK_RPAREN: return "TOK_RPAREN";
        case TOK_LBRACE: return "TOK_LBRACE";
        case TOK_RBRACE: return "TOK_RBRACE";
        case TOK_COMMA: return "TOK_COMMA";
        case TOK_SEMICOLON: return "TOK_SEMICOLON";

        case TOK_ADD: return "TOK_ADD";
        case TOK_SUB: return "TOK_SUB";
        case TOK_MUL: return "TOK_MUL";
        case TOK_DIV: return "TOK_DIV";
        case TOK_MOD: return "TOK_MOD";
        case TOK_INCREMENT: return "TOK_INCREMENT";
        case TOK_DECREMENT: return "TOK_DECREMENT";

        case TOK_EQ: return "TOK_EQ";
        case TOK_NEQ: return "TOK_NEQ";
        case TOK_LT: return "TOK_LT";
        case TOK_LTE: return "TOK_LTE";
        case TOK_GT: return "TOK_GT";
        case TOK_GTE: return "TOK_GTE";

        case TOK_AND: return "TOK_AND";
        case TOK_OR: return "TOK_OR";
        case TOK_NOT: return "TOK_NOT";

        case TOK_ASSIGN: return "TOK_ASSIGN";
        case TOK_PLUS_ASSIGN: return "TOK_PLUS_ASSIGN";
        case TOK_MINUS_ASSIGN: return "TOK_MINUS_ASSIGN";
        case TOK_MUL_ASSIGN: return "TOK_MUL_ASSIGN";
        case TOK_DIV_ASSIGN: return "TOK_DIV_ASSIGN";
        case TOK_MOD_ASSIGN: return "TOK_MOD_ASSIGN";

        case TOK_INT_LIT: return "TOK_INT_LIT";
        case TOK_FLOAT_LIT: return "TOK_FLOAT_LIT";
        case TOK_BOOL_LIT: return "TOK_BOOL_LIT";
        case TOK_CHAR_LIT: return "TOK_CHAR_LIT";

        case TOK_IDENT: return "TOK_IDENT";
        case TOK_EOF: return "TOK_EOF";
        default: return "TOK_UNKNOWN";
    }
}
