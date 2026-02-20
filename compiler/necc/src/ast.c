#include "ast.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Helper macro for allocating memory for AST nodes. Returns NULL on
/// failure.
#define CREATE_NODE(nodeType) \
    ASTNode* node = malloc(sizeof(ASTNode)); \
    if (node == NULL) { \
        fprintf(stderr, "Error: Memory allocation failed for node\n"); \
        return NULL; \
    } \
    node->type = nodeType; \
    node->line = line; \
    node->column = column;

/// Helper macro for allocating memory for string copies and copying
/// strings for AST nodes that have idents.
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

ASTNode* create_file_node(ASTNode** stmts, size_t stmtCount) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for node\n");
        return NULL;
    }

    node->type = NODE_FILE;
    node->line = 1;
    node->column = 1;
    node->data.file.stmts = stmts;
    node->data.file.stmtCount = stmtCount;
    return node;
}

ASTNode* create_function_decl_node(size_t line, size_t column,
    const char* name, ASTNode** params, size_t paramCount,
    TokenType returnType, ASTNode* body) {
    if (!token_is_type(returnType) && returnType != TOK_INVALID) {
        fprintf(stderr, "Error: Invalid return type\n");
        return NULL;
    }

    CREATE_NODE(NODE_FUNCTION_DECL);
    CREATE_STR_COPY(name);

    node->data.functionDecl.name = strCpy;
    node->data.functionDecl.params = params;
    node->data.functionDecl.paramCount = paramCount;
    node->data.functionDecl.returnType = returnType;
    node->data.functionDecl.body = body;
    return node;
}

ASTNode* create_variable_decl_node(size_t line, size_t column,
    const char* name, TokenType type, bool mutable, ASTNode* initializer) {
    if (!token_is_type(type)) {
        fprintf(stderr, "Error: Invalid declaration type\n");
        return NULL;
    }

    CREATE_NODE(NODE_VARIABLE_DECL);
    CREATE_STR_COPY(name);

    node->data.variableDecl.name = strCpy;
    node->data.variableDecl.type = type;
    node->data.variableDecl.mutable = mutable;
    node->data.variableDecl.initializer = initializer;
    return node;
}

ASTNode* create_parameter_decl_node(size_t line, size_t column,
    const char* name, TokenType type) {
    if (!token_is_type(type)) {
        fprintf(stderr, "Error: Invalid parameter type\n");
        return NULL;
    }

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
    if (!token_is_bin_op(op)) {
        fprintf(stderr, "Error: Invalid binary operator\n");
        return NULL;
    }

    CREATE_NODE(NODE_BINARY_EXPR);

    node->data.binaryExpr.op = op;
    node->data.binaryExpr.left = left;
    node->data.binaryExpr.right = right;
    return node;
}

ASTNode* create_unary_expr_node(size_t line, size_t column, TokenType op,
    ASTNode* operand, bool isPostfix) {
    if (!token_is_un_op(op)) {
        fprintf(stderr, "Error: Invalid unary operator\n");
        return NULL;
    }

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
    if (!token_is_assign_op(op)) {
        fprintf(stderr, "Error: Invalid assignment operator\n");
        return NULL;
    }

    CREATE_NODE(NODE_ASSIGN_EXPR);

    node->data.assignExpr.target = target;
    node->data.assignExpr.op = op;
    node->data.assignExpr.value = value;
    return node;
}

ASTNode* create_cast_expr_node(size_t line, size_t column, TokenType type,
    ASTNode* expr) {
    if (!token_is_type(type)) {
        fprintf(stderr, "Error: Invalid cast type\n");
        return NULL;
    }

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
    if (!token_is_literal(type)) {
        fprintf(stderr, "Error: Invalid literal type\n");
        return NULL;
    }

    CREATE_NODE(NODE_LITERAL);
    CREATE_STR_COPY(value);

    node->data.literal.type = type;
    node->data.literal.value = strCpy;
    return node;
}

void free_ast_node(ASTNode* node) {
    if (node == NULL) {
        return;
    }

    switch (node->type) {
        case NODE_FILE:
            if (node->data.file.stmts != NULL) {
                for (size_t i = 0; i < node->data.file.stmtCount; i++) {
                    free_ast_node(node->data.file.stmts[i]);
                }
                free(node->data.file.stmts);
            }
            break;
        case NODE_FUNCTION_DECL:
            if (node->data.functionDecl.name != NULL) {
                free(node->data.functionDecl.name);
            }
            if (node->data.functionDecl.params != NULL) {
                for (size_t i = 0; i < node->data.functionDecl.paramCount;
                    i++) {
                    free_ast_node(node->data.functionDecl.params[i]);
                }
                free(node->data.functionDecl.params);
            }
            free_ast_node(node->data.functionDecl.body);
            break;
        case NODE_VARIABLE_DECL:
            if (node->data.variableDecl.name != NULL) {
                free(node->data.variableDecl.name);
            }
            free_ast_node(node->data.variableDecl.initializer);
            break;
        case NODE_PARAMETER_DECL:
            if (node->data.parameterDecl.name != NULL) {
                free(node->data.parameterDecl.name);
            }
            break;
        case NODE_BLOCK_STMT:
            if (node->data.blockStmt.stmts != NULL) {
                for (size_t i = 0; i < node->data.blockStmt.stmtCount; i++) {
                    free_ast_node(node->data.blockStmt.stmts[i]);
                }
                free(node->data.blockStmt.stmts);
            }
            break;
        case NODE_RETURN_STMT:
            free_ast_node(node->data.returnStmt.expr);
            break;
        case NODE_IF_STMT:
            free_ast_node(node->data.ifStmt.condition);
            free_ast_node(node->data.ifStmt.thenBranch);
            free_ast_node(node->data.ifStmt.elseBranch);
            break;
        case NODE_EXPR_STMT:
            free_ast_node(node->data.exprStmt.expr);
            break;
        case NODE_BINARY_EXPR:
            free_ast_node(node->data.binaryExpr.left);
            free_ast_node(node->data.binaryExpr.right);
            break;
        case NODE_UNARY_EXPR:
            free_ast_node(node->data.unaryExpr.operand);
            break;
        case NODE_CALL_EXPR:
            free_ast_node(node->data.callExpr.callee);
            if (node->data.callExpr.args != NULL) {
                for (size_t i = 0; i < node->data.callExpr.argCount; i++) {
                    free_ast_node(node->data.callExpr.args[i]);
                }
                free(node->data.callExpr.args);
            }
            break;
        case NODE_ASSIGN_EXPR:
            free_ast_node(node->data.assignExpr.target);
            free_ast_node(node->data.assignExpr.value);
            break;
        case NODE_CAST_EXPR:
            free_ast_node(node->data.castExpr.expr);
            break;
        case NODE_IDENT:
            if (node->data.ident.name != NULL) {
                free(node->data.ident.name);
            }
            break;
        case NODE_LITERAL:
            if (node->data.literal.value != NULL) {
                free(node->data.literal.value);
            }
            break;
        default:
            fprintf(stderr, "Warning: Unknown node type %d attempted to be"\
                " freed\n", node->type);
            break;
    }

    free(node);
}

/// Simple helper function to print indentation for AST nodes.
static void print_indent(int indent) {
    if (indent < 0) {
        indent = 0;
    }

    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}

void print_ast_node(ASTNode* node, int indent) {
    if (node == NULL) {
        print_indent(indent);
        printf("(null)\n");
        return;
    }

    print_indent(indent);

    switch (node->type) {
        case NODE_FILE:
            printf("File\n");
            if (node->data.file.stmts != NULL) {
                for (size_t i = 0; i < node->data.file.stmtCount; i++) {
                    print_ast_node(node->data.file.stmts[i], indent + 1);
                }
            }
            break;
        case NODE_FUNCTION_DECL:
            printf("Function(%zu:%zu) name:'", node->line, node->column);

            if (node->data.functionDecl.name != NULL) {
                printf("%s'", node->data.functionDecl.name);
            } else {
                printf("(null)'");
            }

            if (node->data.functionDecl.returnType != TOK_INVALID) {
                printf(" returns:%s\n",
                    token_as_str(node->data.functionDecl.returnType));
            }

            if (node->data.functionDecl.params != NULL) {
                print_indent(indent + 1);
                printf("Parameters:\n");
                for (size_t i = 0; i < node->data.functionDecl.paramCount;
                    i++) {
                    print_ast_node(node->data.functionDecl.params[i],
                        indent + 2);
                }
            }

            if (node->data.functionDecl.body != NULL) {
                print_indent(indent + 1);
                printf("Body:\n");
                print_ast_node(node->data.functionDecl.body, indent + 2);
            }

            break;
        case NODE_VARIABLE_DECL:
            printf("VariableDecl(%zu:%zu) name:'", node->line, node->column);

            if (node->data.variableDecl.name != NULL) {
                printf("%s'", node->data.variableDecl.name);
            } else {
                printf("(null)'");
            }

            printf(" type:%s mutable:",
                token_as_str(node->data.variableDecl.type));

            if (node->data.variableDecl.mutable) {
                printf("true\n");
            } else {
                printf("false\n");
            }

            if (node->data.variableDecl.initializer != NULL) {
                print_indent(indent + 1);
                printf("Initializer:\n");
                print_ast_node(node->data.variableDecl.initializer,
                    indent + 2);
            }

            break;
        case NODE_PARAMETER_DECL:
            printf("ParameterDecl(%zu:%zu) name:'", node->line, node->column);

            if (node->data.parameterDecl.name != NULL) {
                printf("%s'", node->data.parameterDecl.name);
            } else {
                printf("(null)'");
            }

            printf(" type:%s\n", token_as_str(node->data.parameterDecl.type));

            break;
        case NODE_BLOCK_STMT:
            printf("BlockStmt(%zu:%zu)\n", node->line, node->column);

            if (node->data.blockStmt.stmts != NULL) {
                for (size_t i = 0; i < node->data.blockStmt.stmtCount; i++) {
                    print_ast_node(node->data.blockStmt.stmts[i], indent + 1);
                }
            }

            break;
        case NODE_RETURN_STMT:
            printf("ReturnStmt(%zu:%zu)\n", node->line, node->column);

            if (node->data.returnStmt.expr != NULL) {
                print_ast_node(node->data.returnStmt.expr, indent + 1);
            }

            break;
        case NODE_IF_STMT:
            printf("IfStmt(%zu:%zu)\n", node->line, node->column);

            if (node->data.ifStmt.condition != NULL) {
                print_indent(indent + 1);
                printf("Condition:\n");
                print_ast_node(node->data.ifStmt.condition, indent + 2);
            }

            if (node->data.ifStmt.thenBranch != NULL) {
                print_indent(indent + 1);
                printf("Then:\n");
                print_ast_node(node->data.ifStmt.thenBranch, indent + 2);
            }

            if (node->data.ifStmt.elseBranch != NULL) {
                print_indent(indent + 1);
                printf("Else:\n");
                print_ast_node(node->data.ifStmt.elseBranch, indent + 2);
            }

            break;
        case NODE_EXPR_STMT:
            printf("ExprStmt(%zu:%zu)\n", node->line, node->column);

            if (node->data.exprStmt.expr != NULL) {
                print_ast_node(node->data.exprStmt.expr, indent + 1);
            }

            break;
        case NODE_BINARY_EXPR:
            printf("BinaryExpr(%zu:%zu) op:%s\n", node->line, node->column,
                token_as_str(node->data.binaryExpr.op));

            if (node->data.binaryExpr.left != NULL) {
                print_indent(indent + 1);
                printf("Left:\n");
                print_ast_node(node->data.binaryExpr.left, indent + 2);
            }

            if (node->data.binaryExpr.right != NULL) {
                print_indent(indent + 1);
                printf("Right:\n");
                print_ast_node(node->data.binaryExpr.right, indent + 2);
            }

            break;
        case NODE_UNARY_EXPR:
            printf("UnaryExpr(%zu:%zu) op:%s postfix:", node->line,
                node->column, token_as_str(node->data.unaryExpr.op));

            if (node->data.unaryExpr.isPostfix) {
                printf("true\n");
            } else {
                printf("false\n");
            }

            if (node->data.unaryExpr.operand != NULL) {
                print_ast_node(node->data.unaryExpr.operand, indent + 1);
            }

            break;
        case NODE_CALL_EXPR:
            printf("CallExpr(%zu:%zu)\n", node->line, node->column);

            if (node->data.callExpr.callee != NULL) {
                print_indent(indent + 1);
                printf("Callee:\n");
                print_ast_node(node->data.callExpr.callee, indent + 2);
            }

            if (node->data.callExpr.args != NULL) {
                print_indent(indent + 1);
                printf("Arguments:\n");
                for (size_t i = 0; i < node->data.callExpr.argCount; i++) {
                    print_ast_node(node->data.callExpr.args[i], indent + 2);
                }
            }

            break;
        case NODE_ASSIGN_EXPR:
            printf("AssignExpr(%zu:%zu) op:%s\n", node->line, node->column,
                token_as_str(node->data.assignExpr.op));

            if (node->data.assignExpr.target != NULL) {
                print_indent(indent + 1);
                printf("Target:\n");
                print_ast_node(node->data.assignExpr.target, indent + 2);
            }

            if (node->data.assignExpr.value != NULL) {
                print_indent(indent + 1);
                printf("Value:\n");
                print_ast_node(node->data.assignExpr.value, indent + 2);
            }

            break;
        case NODE_CAST_EXPR:
            printf("CastExpr(%zu:%zu) type:%s\n", node->line, node->column,
                token_as_str(node->data.castExpr.type));

            if (node->data.castExpr.expr != NULL) {
                print_ast_node(node->data.castExpr.expr, indent + 1);
            }

            break;
        case NODE_IDENT:
            printf("Ident(%zu:%zu) name:'", node->line, node->column);

            if (node->data.ident.name != NULL) {
                printf("%s'\n", node->data.ident.name);
            } else {
                printf("(null)'\n");
            }

            break;
        case NODE_LITERAL:
            printf("Literal(%zu:%zu) type:%s value:'", node->line,
                node->column, token_as_str(node->data.literal.type));

            if (node->data.literal.value != NULL) {
                printf("%s'\n", node->data.literal.value);
            } else {
                printf("(null)'\n");
            }

            break;
        default:
            printf("Unknown node type %d (%zu:%zu)\n", node->type, node->line,
                node->column);
            break;
    }
}
