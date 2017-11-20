#include <iostream>
#include <stdio.h>
#include "lexicalAnalyzer/LexicalAnalyzer.h"

int main()
{
    char* test = new char[6] { 'p', 'v', 'b', 'e', 'i', 'n' };
    LexicalAnalyzer* l = new LexicalAnalyzer("C://temp//eae.txt");

    while (l->hasMoreSlices())
    {
        SliceType s = l->nextSlice();
        if (s == Number)
            printf("%i", l->getValue());
        else if (s == Identifier)
            printf("%s", l->getName());
    }

    return 0;
}
