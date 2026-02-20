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

/// Represents a file in the abstract syntax tree. This is the root node
/// and contains the declarations of the file.
typedef struct File {
    /// The array of statements in the file.
    ASTNode** stmts;
    /// The number of statements in the file.
    size_t stmtCount;
} File;

typedef struct FunctionDecl {
    /// A null-terminated string representing the name of the function.
    char* name;
    /// The array of parameters in the function declaration. This may be
    /// NULL if the function has no parameters.
    ASTNode** params;
    /// The number of parameters in the function declaration.
    size_t paramCount;
    /// The type of the function's return value. TOK_INVALID if the
    /// function does not have a return type.
    TokenType returnType;
    /// The body of the function.
    ASTNode* body;
} FunctionDecl;

typedef struct VariableDecl {
    /// A null-terminated string representing the name of the variable.
    char* name;
    /// The type of the variable.
    TokenType type;
    /// Whether the variable is mutable.
    bool mutable;
    /// The statement that initializes the variable.
    ASTNode* initializer;
} VariableDecl;

typedef struct ParameterDecl {
    /// A null-terminated string representing the name of the parameter.
    char* name;
    /// The type of the parameter.
    TokenType type;
} ParameterDecl;

typedef struct BlockStmt {
    /// The array of statements in the block.
    ASTNode** stmts;
    /// The number of statements in the block.
    size_t stmtCount;
} BlockStmt;

typedef struct ReturnStmt {
    /// The expression that is returned.
    ASTNode* expr;
} ReturnStmt;

typedef struct IfStmt {
    /// The condition that determines whether the then branch is
    /// executed.
    ASTNode* condition;
    /// The statement that is executed if the condition is true.
    ASTNode* thenBranch;
    /// The statement that is executed if the condition is false. Can be
    /// null if there is no else branch.
    ASTNode* elseBranch;
} IfStmt;

typedef struct ExprStmt {
    /// The expression that is evaluated.
    ASTNode* expr;
} ExprStmt;

typedef struct BinaryExpr {
    /// The type of operator.
    TokenType op;
    /// The left operand of the binary expression.
    ASTNode* left;
    /// The right operand of the binary expression.
    ASTNode* right;
} BinaryExpr;

typedef struct UnaryExpr {
    /// The type of operator.
    TokenType op;
    /// The operand of the unary expression.
    ASTNode* operand;
    /// Whether the unary expression is postfix. True for postfix, false
    /// for prefix.
    bool isPostfix;
} UnaryExpr;

typedef struct CallExpr {
    /// The function being called.
    ASTNode* callee;
    /// The arguments to the function call. This can be NULL if there
    /// are no arguments.
    ASTNode** args;
    /// The number of arguments in the function call.
    size_t argCount;
} CallExpr;

typedef struct AssignExpr {
    /// The target variable of the assignment.
    ASTNode* target;
    /// The type of assignment operator.
    TokenType op;
    /// The value being assigned.
    ASTNode* value;
} AssignExpr;

typedef struct CastExpr {
    /// The type to cast to.
    TokenType type;
    /// The expression to cast.
    ASTNode* expr;
} CastExpr;

typedef struct Ident {
    /// A null-terminated string representing the identifier.
    char* name;
} Ident;

typedef struct Literal {
    /// The type of the literal.
    TokenType type;
    /// A null-terminated string representing the value of the literal.
    char* value;
} Literal;

struct ASTNode {
    /// The type of the node.
    NodeType type;
    /// The line number where the node starts.
    size_t line;
    /// The column number where the node starts.
    size_t column;
    /// The data associated with the node.
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

/// Creates a new file node with the given statements and count. The
/// stmts array must be allocated by the caller but ownership is
/// transferred to the returned node. Returns NULL on failure.
ASTNode* create_file_node(ASTNode** stmts, size_t stmtCount);
/// Creates a new function declaration node with the given name and
/// parameters. The name is expected to be null-terminated and is copied
/// for the node, meaning the caller retains ownership of the name. The
/// params array must be allocated by the caller but ownership is
/// transferred to the returned node. Returns NULL on failure.
ASTNode* create_function_decl_node(size_t line, size_t column,
    const char* name, ASTNode** params, size_t paramCount,
    TokenType returnType, ASTNode* body);
/// Creates a new variable declaration node with the given name and
/// type. The name is expected to be null-terminated and is copied
/// for the node, meaning the caller retains ownership of the name. The
/// initializer must be allocated by the caller but ownership is
/// transferred to the returned node. Returns NULL on failure.
ASTNode* create_variable_decl_node(size_t line, size_t column,
    const char* name, TokenType type, bool mutable, ASTNode* initializer);
/// Creates a new parameter declaration node with the given name and
/// type. The name is expected to be null-terminated and is copied
/// for the node, meaning the caller retains ownership of the name.
/// Returns NULL on failure.
ASTNode* create_parameter_decl_node(size_t line, size_t column,
    const char* name, TokenType type);
/// Creates a new block statement node with the given statements.
/// The stmts array must be allocated by the caller but ownership is
/// transferred to the returned node. Returns NULL on failure.
ASTNode* create_block_stmt_node(size_t line, size_t column, ASTNode** stmts,
    size_t stmtCount);
/// Creates a new return statement node with the given expression.
/// The expr is expected to be allocated by the caller but ownership
/// is transferred to the returned node. Returns NULL on failure.
ASTNode* create_return_stmt_node(size_t line, size_t column, ASTNode* expr);
/// Creates a new if statement node with the given condition, then
/// branch, and else branch. The condition, thenBranch, and elseBranch
/// are expected to be allocated by the caller but ownership is
/// transferred to the returned node. If the statement has no else
/// branch, elseBranch should be NULL. Returns NULL on failure.
ASTNode* create_if_stmt_node(size_t line, size_t column, ASTNode* condition,
    ASTNode* thenBranch, ASTNode* elseBranch);
/// Creates a new expression statement node with the given expression.
/// The expr is expected to be allocated by the caller but ownership
/// is transferred to the returned node. Returns NULL on failure.
ASTNode* create_expr_stmt_node(size_t line, size_t column, ASTNode* expr);
/// Creates a new binary expression node with the given operator, left
/// operand, and right operand. The left and right operands are expected
/// to be allocated by the caller but ownership is transferred to the
/// returned node. Returns NULL on failure.
ASTNode* create_binary_expr_node(size_t line, size_t column, TokenType op,
    ASTNode* left, ASTNode* right);
/// Creates a new unary expression node with the given operator and
/// operand. The operand is expected to be allocated by the caller but
/// ownership is transferred to the returned node. Returns NULL on
/// failure.
ASTNode* create_unary_expr_node(size_t line, size_t column, TokenType op,
    ASTNode* operand, bool isPostfix);
/// Creates a new call expression node with the given callee and
/// arguments. The callee and arguments are expected to be allocated by
/// the caller but ownership is transferred to the returned node.
/// Returns NULL on failure.
ASTNode* create_call_expr_node(size_t line, size_t column, ASTNode* callee,
    ASTNode** args, size_t argCount);
/// Creates a new assignment expression node with the given target,
/// operator, and value. The target and value are expected to be
/// allocated by the caller but ownership is transferred to the returned
/// node. Returns NULL on failure.
ASTNode* create_assign_expr_node(size_t line, size_t column, ASTNode* target,
    TokenType op, ASTNode* value);
/// Creates a new cast expression node with the given type and
/// expression. The type is a TokenType enum value passed by value.
/// The expression is expected to be allocated by the caller but
/// ownership is transferred to the returned node. Returns NULL on
/// failure.
ASTNode* create_cast_expr_node(size_t line, size_t column, TokenType type,
    ASTNode* expr);
/// Creates a new identifier node with the given name. The name is
/// expected to be null-terminated and is copied for the node, meaning
/// the caller retains ownership of the name. Returns NULL on failure.
ASTNode* create_ident_node(size_t line, size_t column, const char* name);
/// Creates a new literal node with the given type and value. The value
/// is expected to be null-terminated and is copied for the node,
/// meaning the caller retains ownership of the value. Returns NULL on
/// failure.
ASTNode* create_literal_node(size_t line, size_t column, TokenType type,
    const char* value);

/// Recursively frees all memory associated with the given AST node.
/// Safely handles NULL.
void free_ast_node(ASTNode* node);
/// Recursively prints the given AST node with the given indentation.
/// Expected to be 0 for the root node.
void print_ast_node(ASTNode* node, int indent);

#endif // AST_H
