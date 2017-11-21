#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include "Lexer.h"

Lexer::Lexer()
{}

Lexer::Lexer(char* c)
{
    this->actualLine = 1;

    FILE* temp = fopen(c, "r");
    if (temp != NULL)
        this->file = fopen(c, "r");
}

SliceType Lexer::nextSlice(bool consume)
{
    std::stringstream test, spaces;
    char c[2];

    if (feof(this->file))
        return Unknown;

    c[0] = fgetc(this->file);
    c[1] = '\0';
    while ((c[0] == ' ' || c[0] == '\n' || c[0] == '\t') && c[0] != EOF)
    {
        spaces << c[0];

        if (c[0] == '\n')
            this->actualLine++;

        c[0] = fgetc(this->file);
    }

    if (c[0] == '.' || c[0] == ';' || c[0] == ':')
        test << c[0];
    else
    {
        while (c[0] != ' ' && c[0] != '\n' && c[0] != '\t' && c[0] != EOF)
        {
            test << c[0];

            //sliceAux = (char*)calloc(strlen(slice), sizeof(char));
            //strcpy(sliceAux, slice);

            //slice = (char*)calloc(strlen(slice) + 1, sizeof(char));
            //strcpy(slice, sliceAux);
            //strcat(slice, &c[0]);

            if (!feof(this->file))
                c[0] = fgetc(this->file);

            if (c[0] == '.' || c[0] == ';' || c[0] == ':')
                break;
        }

        ungetc(c[0], this->file);
    }

    const std::string tmpSlice = test.str();
    const char* sliceStr  = tmpSlice.c_str();

    SliceType ret = Unknown;
    if (!strcmp(sliceStr, "Programm"))
        ret = Program;
    else if (!strcmp(sliceStr, "var"))
        ret = Variable;
    else if (!strcmp(sliceStr, "verfahren"))
        ret = Procedure;
    else if (!strcmp(sliceStr, "funktion"))
        ret = Function;
    else if (!strcmp(sliceStr, "anfangen"))
        ret = Begin;
    else if (!strcmp(sliceStr, "ende"))
        ret = End;
    else if (!strcmp(sliceStr, "."))
        ret = Point;
    else if (!strcmp(sliceStr, ";"))
        ret = Semicolon;
    else if (!strcmp(sliceStr, ":"))
        ret = Colon;
    else
    {
        this->actualSlice = (char*)calloc(strlen(sliceStr), sizeof(char));
        for (int i = 0; i < strlen(sliceStr); i++)
            if (!isdigit(sliceStr[i]))
            {
                strcpy(this->actualSlice, sliceStr);
                ret = Identifier;
                break;
            }

        if (ret == Unknown)
        {
            strcpy(this->actualSlice, sliceStr);
            ret = Number;
        }
    }

    if (!consume)
    {
        for (int n = strlen(sliceStr) - 1; n >= 0; n--)
            ungetc(sliceStr[n], this->file);

        const std::string tmpSpace = spaces.str();
        const char* spacesStr = tmpSpace.c_str();
        for (int n = strlen(spacesStr) - 1; n >= 0; n--)
        {
            if (spacesStr[n] == '\n')
                this->actualLine--;

            ungetc(spacesStr[n], this->file);
        }
    }

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

void Lexer::throwError(char* msg)
{
    std::stringstream sstm;
    sstm << "Exception on line " << this->actualLine << ": " << msg;

    throw std::invalid_argument(sstm.str());
}
