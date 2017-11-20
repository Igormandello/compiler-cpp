#include <iostream>
#include <stdio.h>
#include "lexer/Lexer.h"
#include "parser/Parser.h"

int main()
{
    Lexer  l ("C://temp//eae.txt");

    while (l.hasMoreSlices())
    {
        SliceType s = l.nextSlice(true);
        if (s == Number)
            printf("%i", l.getValue());
        else if (s == Identifier)
            printf("%s", l.getName());
    }

    return 0;
}
