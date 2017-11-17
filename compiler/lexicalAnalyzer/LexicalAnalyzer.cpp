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
    char* sliceAux;
    char* slice = (char*)calloc(1, sizeof(char));
    char c[2];
    int x = 1;

    c[0] = fgetc(this->file);
    c[1] = '\0';
    while ((c[0] == ' ' || c[0] == '\n' || c[0] == '\t') && c[0] != EOF)
        c[0] = fgetc(this->file);

    while (c[0] != ' ' && c[0] != '\n' && c[0] != '\t' && c[0] != EOF)
    {
        sliceAux = (char*)calloc(x++, sizeof(char));
        strcpy(sliceAux, slice);

        slice = (char*)calloc(x, sizeof(char));
        strcpy(slice, sliceAux);
        strcat(slice, &c[0]);

        if (!feof(this->file))
            c[0] = fgetc(this->file);
    }

    SliceType ret = Unknown;
    if (!strcmp(slice, "programm"))
        ret = Program;
    else if (!strcmp(slice, "var"))
        ret = Variable;
    else if (!strcmp(slice, "anfangen"))
        ret = Begin;
    else if (!strcmp(slice, "ende"))
        ret = End;
    else
    {
        for (int i = 0; i < strlen(slice); i++)
            if (!isdigit(slice[i]))
                ret = Identifier;

        if (ret == Unknown)
            ret = Number;
    }

    free(slice);
    return ret;
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
