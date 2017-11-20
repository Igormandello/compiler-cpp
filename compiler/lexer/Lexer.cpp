#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Lexer.h"

Lexer::Lexer(char* c)
{
    FILE* temp = fopen(c, "r");

    if (temp != NULL)
        this->file = fopen(c, "r");
}

SliceType Lexer::nextSlice(bool consume)
{
    free(this->actualSlice);

    char* sliceAux,
        * slice = (char*)calloc(1, sizeof(char)),
        * spaces = (char*)calloc(1, sizeof(char));
    char c[2];

    c[0] = fgetc(this->file);
    c[1] = '\0';
    while ((c[0] == ' ' || c[0] == '\n' || c[0] == '\t') && c[0] != EOF)
    {
        sliceAux = (char*)calloc(strlen(spaces), sizeof(char));
        strcpy(sliceAux, spaces);

        spaces = (char*)calloc(strlen(spaces) + 1, sizeof(char));
        strcpy(spaces, sliceAux);
        strcat(spaces, &c[0]);

        c[0] = fgetc(this->file);
    }

    while (c[0] != ' ' && c[0] != '\n' && c[0] != '\t' && c[0] != EOF)
    {
        sliceAux = (char*)calloc(strlen(slice), sizeof(char));
        strcpy(sliceAux, slice);

        slice = (char*)calloc(strlen(slice) + 1, sizeof(char));
        strcpy(slice, sliceAux);
        strcat(slice, &c[0]);

        if (!feof(this->file))
            c[0] = fgetc(this->file);
    }
    ungetc(c[0], this->file);

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
        this->actualSlice = (char*)calloc(strlen(slice), sizeof(char));
        for (int i = 0; i < strlen(slice); i++)
            if (!isdigit(slice[i]))
            {
                strcpy(this->actualSlice, slice);
                ret = Identifier;
                break;
            }

        if (ret == Unknown)
        {
            strcpy(this->actualSlice, slice);
            ret = Number;
        }
    }

    if (!consume)
    {
        for (int n = strlen(slice) - 1; n >= 0; n--)
            ungetc(slice[n], this->file);

        for (int n = strlen(spaces) - 1; n >= 0; n--)
            ungetc(spaces[n], this->file);
    }

    free(spaces);
    free(sliceAux);
    free(slice);
    return ret;
}

bool Lexer::hasMoreSlices()
{
    char c = fgetc(this->file);

    if (feof(file))
    {
        ungetc(c, this->file);
        return false;
    }

    ungetc(c, this->file);
    return true;
}

char* Lexer::getName()
{
    return this->actualSlice;
}

int Lexer::getValue()
{
    return atoi(this->actualSlice);
}
