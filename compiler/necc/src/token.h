#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>
#include <stdbool.h>

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
    TOK_LBRACE,
    TOK_RBRACE,
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
    /// Type of token.
    TokenType type;
    /// Null-terminated identifier associated with the token,
    /// can be NULL.
    /// For identifiers, this is the name.
    /// For boolean literals, this is the string "true" or "false".
    /// For numeric literals, this is the literal.
    /// For character literals, this is the character.
    char* ident;
    /// Line number where the token started.
    size_t line;
    /// Column number where the token started.
    size_t column;
} Token;

/// Creates a new token, identifier can be NULL but if not, it is
/// expected to have been allocated by the caller and ownership is
/// transferred to the token. Call free_token() to free the token and
/// identifier if necessary. Returns a pointer to the newly created
/// token, or NULL if memory allocation fails.
Token* create_token(TokenType type, char* ident, size_t line, size_t column);
/// Frees a token and its identifier if it exists.
void free_token(Token *token);
/// Simple helper function to convert a token type to a string
/// representation, useful for debugging and error messages. Returns
/// a string representation of the token type, or "TOK_UNKNOWN" if the
/// type is not recognized.
const char* token_as_str(TokenType type);
/// Returns true if the token type is a type.
bool token_is_type(TokenType type);
/// Returns true if the token type is a binary operator.
bool token_is_bin_op(TokenType type);
/// Returns true if the token type is a unary operator.
bool token_is_un_op(TokenType type);
/// Returns true if the token type is an assignment operator.
bool token_is_assign_op(TokenType type);
/// Returns true if the token type is a literal.
bool token_is_literal(TokenType type);

#endif // TOKEN_H
