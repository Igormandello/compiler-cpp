#include <iostream>
#include <stdio.h>
#include "lexer/Lexer.h"
#include "parser/Parser.h"

using namespace std;
int main()
{
    Lexer  l ("C://temp//eae.txt");
    Parser p (l);

    try
    {
        p.compileProgramInit();

        SliceType next = l.nextSlice(false);
        if(next == Variable)
        {
           p.compileVariable();
           next = l.nextSlice(false);
        }
        //main loop
        //switch(next)
        //{
        //  Procedure: p.compileProcedure();
        //  Function : p.compileFunction();
        //}
        //prox = l.nextSlice(false);
        //loop end
    }
    catch (std::exception const& e)
    {
        cerr << "Exception: " << e.what() << endl;
    }
    cout << "So far, so good";

    return 0;
}
