#include <iostream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "symbols/Parameter.h"

using namespace std;
int main()
{
    std::string fileName;
    cin >> fileName;

    try
    {
        Parser p ((char*)fileName.c_str());

        p.compileProgram();
        cout << "So far, so good";
    }
    catch (std::exception const &e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}
