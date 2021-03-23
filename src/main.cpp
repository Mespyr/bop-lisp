#include "../include/util.h"
#include "../include/parser.h"
#include "../include/eval.h"

// Print AST for debugging
void ast_out(std::vector<Node> nodes, std::string indent = "") {
    for (int i = 0; i < (int) nodes.size(); i++) {
        Node node = nodes.at(i);
        if (node.type == ATOM) {
            std::cout << indent << "TYPE: " << node.atom.type << ", LINE: " << node.atom.line << 
                ", VALUE: " << node.atom.value << std::endl;
        } else {
            std::cout << indent << "NODE:\n";
            ast_out(node.nodes, indent+"    ");
        }
    }
}


// Evaluate String
int evalString(std::string code) {
    TokenList tokens = tokenize(code);
    AST ast = parse(tokens);
    if (ast.error_found) {
        std::cout << ast.error.error() << std::endl;
        return 1;
    }
    AST_Handler ast_buf{ast};
    Evaluator eval;

    while(!ast_buf.eof) {
        Node node = ast_buf.next();
        // ast_out(node.nodes);
        eval.evaluate(node);
        if (eval.error_found) {
            std::cout << eval.error.error() << std::endl;
            ast_buf.eof = true;
        }
    }
    return 0;
}

// REPL for Bop-lisp
void REPL() {  // This function isn't used. It will be implimented after writing the evaluator
    printf("Bop Version 1.0.0\n");
    printf("Press 'Ctrl+C' to exit.\n\n");

    while (1) {
        printf("USER> ");
        std::string code;
        getline(std::cin, code);
        code += " "; 
        evalString(code);
    }
}

// Main
int main(int argc, char const *argv[])
{   
    if (argc <2) { 
        // REPL();
        return 1;
    } else {
        std::string code = read_ascii_file(argv[1]);
        return evalString(code);
    }
}