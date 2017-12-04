#include <iostream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"

using namespace std;
int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
            throw std::invalid_argument("Usage: compiler.exe <filename>");

        Parser p (argv[1]);

        p.compileProgram();
        cout << "So far, so good";
    }
    catch (std::exception const &e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}
