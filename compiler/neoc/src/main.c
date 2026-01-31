#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "lexer.h"

#define TEST_SRC "fn main() i32 {\n return 0; \n} //* Comment Test */"
#define TEST_OPS "+ - * / % ++ -- == != < <= > >= && || ! = += -= *=  \
                    /= %="
#define TEST_PUNCT " ( ) { } , ;"
#define TEST_WORDS "fn return mut if else i8 i16 i32 i64 i128 u8 u16 \
                        u32 u64 u128 f32 f64 bool char true false word"
#define TEST_NUM "22000 001 100 222 323 4 5 6 7 8 9 10.0 111.10200323 \
                    25.0000 24.4.4.0 25 .5 . 5. 5.. .. //*"
#define TEST_CHAR "'a' 'b' 'c' 'b ' 'bc '' ''c'  "

int main(void) {
    // Have to create a heap allocated version of our test src code
    // otherwise it will crash when the lexer tries to free it
    char* src = malloc(strlen(TEST_CHAR) + 1);
    if (src == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(src, TEST_CHAR);

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
