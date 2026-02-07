#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CREATE_NODE(nodeType) \
    ASTNode* node = malloc(sizeof(ASTNode)); \
    if (node == NULL) { \
        fprintf(stderr, "Error: Memory allocation failed for node\n"); \
        return NULL; \
    } \
    node->type = nodeType; \
    node->line = line; \
    node->column = column;

#define CREATE_STR_COPY(str) \
    if (str == NULL) { \
        fprintf(stderr, "Error: String passed to node cannot" \
        " be NULL\n"); \
        free_ast_node(node); \
        return NULL; \
    } \
    size_t len = strlen(str); \
    char* strCpy = malloc(len + 1); \
    if (strCpy == NULL) { \
        fprintf(stderr, "Error: Memory allocation failed\n"); \
        free_ast_node(node); \
        return NULL; \
    } \
    memcpy(strCpy, str, len); \
    strCpy[len] = '\0';

ASTNode* create_file_node(size_t line, size_t column, ASTNode** stmts,
    size_t stmtCount) {
    CREATE_NODE(NODE_FILE);

    node->data.file.stmts = stmts;
    node->data.file.stmtCount = stmtCount;
    return node;
}

ASTNode* create_function_decl_node(size_t line, size_t column, const char* name,
    ASTNode** params, size_t paramCount, TokenType returnType, ASTNode* body) {
    CREATE_NODE(NODE_FUNCTION_DECL);
    CREATE_STR_COPY(name);

    node->data.functionDecl.name = strCpy;
    node->data.functionDecl.params = params;
    node->data.functionDecl.paramCount = paramCount;
    node->data.functionDecl.returnType = returnType;
    node->data.functionDecl.body = body;
    return node;
}

ASTNode* create_variable_decl_node(size_t line, size_t column, const char* name,
    TokenType type, bool mutable, ASTNode* initializer) {
    CREATE_NODE(NODE_VARIABLE_DECL);
    CREATE_STR_COPY(name);

    node->data.variableDecl.name = strCpy;
    node->data.variableDecl.type = type;
    node->data.variableDecl.mutable = mutable;
    node->data.variableDecl.initializer = initializer;
    return node;
}

ASTNode* create_parameter_decl_node(size_t line, size_t column, const char* name,
    TokenType type) {
    CREATE_NODE(NODE_PARAMETER_DECL);
    CREATE_STR_COPY(name);

    node->data.parameterDecl.name = strCpy;
    node->data.parameterDecl.type = type;
    return node;
}

ASTNode* create_block_stmt_node(size_t line, size_t column, ASTNode** stmts,
    size_t stmtCount) {
    CREATE_NODE(NODE_BLOCK_STMT);

    node->data.blockStmt.stmts = stmts;
    node->data.blockStmt.stmtCount = stmtCount;
    return node;
}

ASTNode* create_return_stmt_node(size_t line, size_t column, ASTNode* expr) {
    CREATE_NODE(NODE_RETURN_STMT);

    node->data.returnStmt.expr = expr;
    return node;
}

ASTNode* create_if_stmt_node(size_t line, size_t column, ASTNode* condition,
    ASTNode* thenBranch, ASTNode* elseBranch) {
    CREATE_NODE(NODE_IF_STMT);

    node->data.ifStmt.condition = condition;
    node->data.ifStmt.thenBranch = thenBranch;
    node->data.ifStmt.elseBranch = elseBranch;
    return node;
}

ASTNode* create_expr_stmt_node(size_t line, size_t column, ASTNode* expr) {
    CREATE_NODE(NODE_EXPR_STMT);

    node->data.exprStmt.expr = expr;
    return node;
}

ASTNode* create_binary_expr_node(size_t line, size_t column, TokenType op,
    ASTNode* left, ASTNode* right) {
    CREATE_NODE(NODE_BINARY_EXPR);

    node->data.binaryExpr.op = op;
    node->data.binaryExpr.left = left;
    node->data.binaryExpr.right = right;
    return node;
}

ASTNode* create_unary_expr_node(size_t line, size_t column, TokenType op,
    ASTNode* operand, bool isPostfix) {
    CREATE_NODE(NODE_UNARY_EXPR);

    node->data.unaryExpr.op = op;
    node->data.unaryExpr.operand = operand;
    node->data.unaryExpr.isPostfix = isPostfix;
    return node;
}

ASTNode* create_call_expr_node(size_t line, size_t column, ASTNode* callee,
    ASTNode** args, size_t argCount) {
    CREATE_NODE(NODE_CALL_EXPR);

    node->data.callExpr.callee = callee;
    node->data.callExpr.args = args;
    node->data.callExpr.argCount = argCount;
    return node;
}

ASTNode* create_assign_expr_node(size_t line, size_t column, ASTNode* target,
    TokenType op, ASTNode* value) {
    CREATE_NODE(NODE_ASSIGN_EXPR);

    node->data.assignExpr.target = target;
    node->data.assignExpr.op = op;
    node->data.assignExpr.value = value;
    return node;
}

ASTNode* create_cast_expr_node(size_t line, size_t column, TokenType type,
    ASTNode* expr) {
    CREATE_NODE(NODE_CAST_EXPR);

    node->data.castExpr.type = type;
    node->data.castExpr.expr = expr;
    return node;
}

ASTNode* create_ident_node(size_t line, size_t column, const char* name) {
    CREATE_NODE(NODE_IDENT);
    CREATE_STR_COPY(name);

    node->data.ident.name = strCpy;
    return node;
}

ASTNode* create_literal_node(size_t line, size_t column, TokenType type,
    const char* value) {
    CREATE_NODE(NODE_LITERAL);
    CREATE_STR_COPY(value);

    node->data.literal.type = type;
    node->data.literal.value = strCpy;
    return node;
}

void free_ast_node(ASTNode* node) {
    return;
}

void print_ast_node(ASTNode* node, int indent);
