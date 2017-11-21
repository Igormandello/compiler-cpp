#include <iostream>
#include <stdio.h>
#include "lexer/Lexer.h"
#include "parser/Parser.h"

int main()
{
    Lexer  l ("C://temp//eae.txt");
    Parser p (l);

    p.compileProgramInit();
    SliceType next = l.nextSlice(false);
    //if(next == Variable)
    //{
    //   p.compileVariable();
    //   next = l.nextSlice(false);
    //}
    //main loop
    //switch(next)
    //{
    //  Procedure: p.compileProcedure();
    //  Function : p.compileFunction();
    //}
    //prox = l.nextSlice(false);
    //loop end

    return 0;
}
