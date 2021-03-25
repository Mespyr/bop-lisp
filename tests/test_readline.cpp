#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char** argv) {
    printf("Welcome! You can exit by pressing Ctrl+C at any time...\n");

    rl_bind_key('\t', rl_insert);
    while (1) {
        std::string buf = readline(">> ");
        if (buf.length() > 0) {
            add_history(buf.c_str());
        }
        std::cout << "Hello, " << buf << std::endl;
    }

    return 0;
}