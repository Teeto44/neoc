#include "token.h"
#include "lexer.h"

int main(int argc, char** argv) {
    lexer_open_src("fn main() i32 {\n return 0; \n}");

    Token* token = get_next_token();
    while (token->type != TOK_EOF) {
        token = get_next_token();
    }

    printf("EOF reached \n");

    lexer_close_src();

    return 0;
}
