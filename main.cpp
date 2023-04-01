#include <iostream>
#include "ast.h"
#include "parser.cpp"
#include "ConstantPropagation/ConstantPropagation.h"

using namespace std;

int main(){
    string tests[6];

    tests[0] += "x = 10;";

    tests[1] += "x = 10 + 20;\n";
    tests[1] += "y = x + 10 / 20 - x;";
    
    tests[2] += "a = 3 + 5; \n";
    tests[2] += "if a > 5 \n";
    tests[2] += "{ b = 10; } \n";
    tests[2] += "else \n";
    tests[2] += "{ b = 0; }; \n";
    tests[2] += "c = a + b - a - b;";

    tests[3] += "x = 5; \n";
    tests[3] += "y = 0; \n";
    tests[3] += "while x > 0 { \n";
    tests[3] += "  y = y + 2; \n";
    tests[3] += "  x = x - 1;}; \n";
    tests[3] += "z = y * 3;";

    tests[4] += "while true { \n";
    tests[4] += "  if true { \n";
    tests[4] += "    while false { \n";
    tests[4] += "       x = 1; \n";
    tests[4] += "    } \n";
    tests[4] += "  } \n";
    tests[4] += "  else{ \n";
    tests[4] += "    y = 2; \n";
    tests[4] += "  } \n";
    tests[4] += "}; \n";
    tests[4] += "z = 3;";


    tests[5] += "a = 2; \n";
    tests[5] += "if true \n";
    tests[5] += "{a = 3;} \n";
    tests[5] += "else \n";
    tests[5] += "{b = 3;}; \n";
    tests[5] += "b = 4;";

    // if you want to test them one by one
    // change this value to needed test
    // if want to check all tests, make it equal to -1

    int test = -1;
    if (test == -1) {
        for (int i = 0; i < 6; ++i) {
            cout << "\n--------------------------------------------\n";
            string input = tests[i];
            cout << "Our test with number " << i << " is\n";
            cout << input << "\n\n";
            cout << "And here is its AST\n";
            yy_scan_string(input.c_str());
            yyparse();
            print_ast(root);
            cout << "\n--------------------------------------------\n";
            ConstantPropagation* opt = new ConstantPropagation();
            root->accept(opt);
            opt->printStates();
            cout << "\n--------------------------------------------\n";
        }
    }
    else if (0 <= test && test <= 5) {
        cout << "\n--------------------------------------------\n";
        string input = tests[test];
        cout << "Our test with number " << test << " is\n";
        cout << input << "\n\n";
        cout << "And here is its AST\n";
        yy_scan_string(input.c_str());
        yyparse();
        print_ast(root);
        cout << "\n--------------------------------------------\n";
        ConstantPropagation* opt = new ConstantPropagation();
        root->accept(opt);
        opt->printStates();
        cout << "\n--------------------------------------------\n";
    } else {
        cout << "Read comments one more time\n";
    }

    return 0;
}