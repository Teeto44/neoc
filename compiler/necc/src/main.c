#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "lexer.h"

int main(int argc, char* argv[]) {
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

    Lexer* lexer = create_lexer(src);
    if (lexer == NULL) {
        fprintf(stderr, "Error: Lexer creation failed\n");
        free(src);
        exit(EXIT_FAILURE);
    }

    while (true) {
        Token* token = get_next_token(lexer);
        if (token == NULL) {
            fprintf(stderr, "Error: Token is NULL\n");
            destroy_lexer(lexer);
            exit(EXIT_FAILURE);
        }

        printf("%s", token_as_str(token->type));
        if (token->ident != NULL) {
            printf(" (%s)", token->ident);
        }
        printf("\n");

        if (token->type == TOK_EOF ) {
            free_token(token);
            break;
        }

        free_token(token);
    }

    destroy_lexer(lexer);

    return 0;
}
