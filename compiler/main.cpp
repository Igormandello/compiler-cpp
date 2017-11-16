#include <iostream>
#include <stdio.h>
#include "lexicalAnalyzer/LexicalAnalyzer.h"

int main()
{
    LexicalAnalyzer* l = new LexicalAnalyzer("C://temp//eae.txt");
    printf("%i", l->hasMoreSlices());

    printf("\n%s", (l->nextSlice() == Unknown ? "eae" : "opa"));
    return 0;
}
