#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(char* c)
{
    FILE* temp = fopen(c, "r");

    if (temp != NULL)
        this->file = fopen(c, "r");
}

SliceType LexicalAnalyzer::nextSlice()
{
    char* slice = (char*)calloc(1, sizeof(char));
    char c[2];

    c[0] = fgetc(this->file);
    c[1] = '\0';
    while (c[0] != ' ' && c[0] != '\n' && c[0] != '\t' && c[0] != EOF)
    {
        strcat(slice, &c[0]);

        if (!feof(this->file))
            c[0] = fgetc(this->file);
    }

    if (!strcmp(slice, "programm")
        return Program;
    else if (!strcmp(slice, "variable"))
        return Variable;
    else if (!strcmp(slice, "begin"))
        return Begin;
    else if (!strcmp(slice, "ende"))
        return End;
    else
    {
        for (int i = 0; i < strlen(slice); i++)
            if (!isdigit(slice[i]))
                return Identifier;

        return Number;
    }
}

char LexicalAnalyzer::hasMoreSlices()
{
    char c = fgetc(this->file);

    if (feof(file))
    {
        ungetc(c, this->file);
        return 0;
    }

    ungetc(c, this->file);
    return 1;
}

char* LexicalAnalyzer::getName()
{

}

int LexicalAnalyzer::getValue()
{

}
