#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "lexer.h"

#define TEST_SRC "fn main() i32 {\n return 0; \n} //* Comment Test */"
#define TEST_OPS "+ - * / % ++ -- == != < <= > >= && || ! = += -= *=  \
                    /= %="

int main(void) {
    // Have to create a heap allocated version of our test src code
    // otherwise it will crash when the lexer tries to free it
    char* src = malloc(strlen(TEST_OPS) + 1);
    if (src == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(src, TEST_OPS);

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
            exit(EXIT_FAILURE);
        }

        printf("%s\n", token_as_str(token->type));

        if (token->type == TOK_EOF ) {
            free_token(token);
            break;
        }

        free_token(token);
    }

    destroy_lexer(lexer);

    return 0;
}
