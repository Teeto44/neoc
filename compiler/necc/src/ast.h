#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stddef.h>
#include "token.h"

typedef enum NodeType {
    // Root
    NODE_FILE,

    // Declarations
    NODE_FUNCTION_DECL,
    NODE_VARIABLE_DECL,
    NODE_PARAMETER_DECL,

    // Statements
    NODE_BLOCK_STMT,
    NODE_RETURN_STMT,
    NODE_IF_STMT,
    NODE_EXPR_STMT,

    // Expressions
    NODE_BINARY_EXPR,
    NODE_UNARY_EXPR,
    NODE_CALL_EXPR,
    NODE_ASSIGN_EXPR,
    NODE_CAST_EXPR,

    // Literals
    NODE_IDENT,
    NODE_LITERAL,
} NodeType;

typedef struct ASTNode ASTNode;

typedef struct File {
    ASTNode** stmts;
    size_t stmtCount;
} File;

typedef struct FunctionDecl {
    char* name;
    ASTNode** params;
    size_t paramCount;
    TokenType returnType;
    ASTNode* body;
} FunctionDecl;

typedef struct VariableDecl {
    char* name;
    TokenType type;
    bool mutable;
    ASTNode* initializer;
} VariableDecl;

typedef struct ParameterDecl {
    char* name;
    TokenType type;
} ParameterDecl;

typedef struct BlockStmt {
    ASTNode** stmts;
    size_t stmtCount;
} BlockStmt;

typedef struct ReturnStmt {
    ASTNode* expr;
} ReturnStmt;

typedef struct IfStmt {
    ASTNode* condition;
    ASTNode* thenBranch;
    ASTNode* elseBranch;
} IfStmt;

typedef struct ExprStmt {
    ASTNode* expr;
} ExprStmt;

typedef struct BinaryExpr {
    TokenType op;
    ASTNode* left;
    ASTNode* right;
} BinaryExpr;

typedef struct UnaryExpr {
    TokenType op;
    ASTNode* operand;
    bool isPostfix;
} UnaryExpr;

typedef struct CallExpr {
    ASTNode* callee;
    ASTNode** args;
    size_t argCount;
} CallExpr;

typedef struct AssignExpr {
    ASTNode* target;
    TokenType op;
    ASTNode* value;
} AssignExpr;

typedef struct CastExpr {
    TokenType type;
    ASTNode* expr;
} CastExpr;

typedef struct Ident {
    char* name;
} Ident;

typedef struct Literal {
    TokenType type;
    char* value;
} Literal;

struct ASTNode {
    NodeType type;
    size_t line;
    size_t column;
    union {
        File file;
        FunctionDecl functionDecl;
        VariableDecl variableDecl;
        ParameterDecl parameterDecl;
        BlockStmt blockStmt;
        ReturnStmt returnStmt;
        IfStmt ifStmt;
        ExprStmt exprStmt;
        BinaryExpr binaryExpr;
        UnaryExpr unaryExpr;
        CallExpr callExpr;
        AssignExpr assignExpr;
        CastExpr castExpr;
        Ident ident;
        Literal literal;
    } data;
};

ASTNode* create_file_node(ASTNode** stmts, size_t stmtCount);
ASTNode* create_function_decl_node(size_t line, size_t column,
    const char* name, ASTNode** params, size_t paramCount,
    TokenType returnType, ASTNode* body);
ASTNode* create_variable_decl_node(size_t line, size_t column,
    const char* name, TokenType type, bool mutable, ASTNode* initializer);
ASTNode* create_parameter_decl_node(size_t line, size_t column,
    const char* name, TokenType type);
ASTNode* create_block_stmt_node(size_t line, size_t column, ASTNode** stmts,
    size_t stmtCount);
ASTNode* create_return_stmt_node(size_t line, size_t column, ASTNode* expr);
ASTNode* create_if_stmt_node(size_t line, size_t column, ASTNode* condition,
    ASTNode* thenBranch, ASTNode* elseBranch);
ASTNode* create_expr_stmt_node(size_t line, size_t column, ASTNode* expr);
ASTNode* create_binary_expr_node(size_t line, size_t column, TokenType op,
    ASTNode* left, ASTNode* right);
ASTNode* create_unary_expr_node(size_t line, size_t column, TokenType op,
    ASTNode* operand, bool isPostfix);
ASTNode* create_call_expr_node(size_t line, size_t column, ASTNode* callee,
    ASTNode** args, size_t argCount);
ASTNode* create_assign_expr_node(size_t line, size_t column, ASTNode* target,
    TokenType op, ASTNode* value);
ASTNode* create_cast_expr_node(size_t line, size_t column, TokenType type,
    ASTNode* expr);
ASTNode* create_ident_node(size_t line, size_t column, const char* name);
ASTNode* create_literal_node(size_t line, size_t column, TokenType type,
    const char* value);

void free_ast_node(ASTNode* node);
void print_ast_node(ASTNode* node, int indent);

#endif // AST_H
