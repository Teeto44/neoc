#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "lexer.h"

#define TEST_SRC "fn main() i32 {\n return 0; \n} //* Comment Test */"
#define TEST_OPS "+ - * / % ++ -- == != < <= > >= && || ! = += -= *=  \
                    /= %="

int main(void) {
    lexer_open_src(TEST_OPS);

    while (true) {
        Token* token = get_next_token();
        if (token == NULL) {
            fprintf(stderr, "Error: Token is NULL");
            exit(EXIT_FAILURE);
        }

        printf("%s\n", token_as_str(token->type));

        if (token->type == TOK_EOF ) {
            break;
        }

        free_token(token);
    }

    lexer_close_src();

    return 0;
}
