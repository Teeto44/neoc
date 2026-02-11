#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "token.h"

int main(void/*int argc, char* argv[]*/) {
    /*
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error: Failed to open input file\n");
        exit(EXIT_FAILURE);
    }

    fseek(input_file, 0, SEEK_END);
    size_t file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    char* src = malloc(file_size + 1);
    if (src == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(input_file);
        exit(EXIT_FAILURE);
    }
    fread(src, 1, file_size, input_file);
    src[file_size] = '\0';
    fclose(input_file);
    free(src);
    */

    // Create an example AST for testing
    ASTNode* num1 = create_literal_node(10, 9, TOK_INT_LIT, "42");
    ASTNode* num2 = create_literal_node(10, 9, TOK_INT_LIT, "20");
    ASTNode* add_expr = create_binary_expr_node(10, 9, TOK_ADD, num1, num2);
    ASTNode* var_decl = create_variable_decl_node(10, 10, "my_var", TOK_I32,
        false, add_expr);
    ASTNode* body = create_block_stmt_node(10, 10, &var_decl, 1);
    ASTNode* argcDecl = create_parameter_decl_node(100, 200, "argc", TOK_I32);
    ASTNode* func_decl = create_function_decl_node(10, 10, "main", &argcDecl,
        1, TOK_I32, body);
    ASTNode* file_node = create_file_node(&func_decl, 1);

    print_ast_node(file_node, 0);

    // This errors at runtime because the hardcoded identifier's are
    // not on the heap, but its ok for testing purposes
    free_ast_node(file_node);
    return 0;
}
