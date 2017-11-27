#include <iostream>
#include <stdio.h>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "symbols/Parameter.h"

using namespace std;
int main()
{
    bool error = false;
    Parser p ("C://temp//Vierte.Reich");

    try
    {
        p.compileProgram();
    }
    catch (std::exception const &e)
    {
        error = true;
        cerr << e.what() << endl;
    }

    if (!error)
        cout << "So far, so good";

    return 0;
}
