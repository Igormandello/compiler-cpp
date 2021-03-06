#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include "Lexer.h"

char* Lexer::reserved[] =
{
    "programm",   //Program
    "var",        //Variable
    "verfahren",  //Procedure
    "funktion",   //Function
    "ob",         //If
    "dann",       //Then
    "sonst",      //Else
    "solange",    //While
    "machen",     //Do
    "anfagen",    //Begin
    "ende",       //End
    ".",
    ";",
    ":",
    ",",
    "(",
    ")",
    "oder",       //Or
    "und",        //And
    "=",
    ">",
    "<",
    "nicht",      //Not
    "+",
    "-",
    "*",
    "/",
    "schreiben",  //Write
    "lesen",      //Read
    "wahr",       //True
    "unwahr",     //False
    "ganze",      //Integer
    "boolescher", //Boolean
    "nichtig"     //Null
};

char Lexer::symbols[] =
{
    '.',
    ';',
    ':',
    ',',
    '(',
    ')',
    '=',
    '>',
    '<',
    '+',
    '-',
    '*',
    '/'
};

Lexer::Lexer(char* c) throw(std::invalid_argument)
{
    this->actualLine = 1;

    int len = strlen(c);
    if ((char)tolower(c[len - 1]) != 'h' || (char)tolower(c[len - 2]) != 'c' ||
        (char)tolower(c[len - 3]) != 'r' || (char)tolower(c[len - 4]) != '.')
        throw std::invalid_argument("Invalid file extension");

    FILE* temp = fopen(c, "r");
    if (temp != NULL)
        this->file = fopen(c, "r");
    else
        throw std::invalid_argument("Invalid file");
}

Lexer::Lexer(const Lexer& l) throw()
{
    this->actualLine = l.actualLine;
    this->file = l.file;
}

Lexer::~Lexer() throw()
{
    free(this->actualSlice);
    fclose(this->file);
}

SliceType Lexer::nextSlice(bool consume)  throw()
{
    std::stringstream slice, spaces;
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

    bool isSymbol = false;
    for (int n = 0; n < SYMBOLS_LENGHT; n++)
        if (c[0] == Lexer::symbols[n])
        {
            slice << (char)tolower(c[0]);
            isSymbol = true;
            break;
        }

    if (!isSymbol)
    {
        while (c[0] != ' ' && c[0] != '\n' && c[0] != '\t' && c[0] != EOF)
        {
            slice << (char)tolower(c[0]);

            if (!feof(this->file))
                c[0] = fgetc(this->file);

            for (int n = 0; n < SYMBOLS_LENGHT; n++)
                if (c[0] == Lexer::symbols[n])
                {
                    isSymbol = true;
                    break;
                }

            if (isSymbol)
                break;
        }

        ungetc(c[0], this->file);
    }

    const std::string tmpSlice = slice.str();
    const char* sliceStr  = tmpSlice.c_str();

    SliceType ret = Unknown;
    for (int n = 0; n < RESERVED_LENGHT; n++)
        if (!strcmp(sliceStr, Lexer::reserved[n]))
        {
            ret = (SliceType)n;
            break;
        }

    if (ret == Unknown)
    {
        this->actualSlice = (char*)calloc(strlen(sliceStr), sizeof(char));
        strcpy(this->actualSlice, sliceStr);

        for (int i = 0; i < strlen(sliceStr); i++)
            if (!isdigit(sliceStr[i]))
            {
                ret = Identifier;
                break;
            }

        if (ret == Unknown)
            ret = Number;
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

bool Lexer::hasMoreSlices() const throw()
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

char* Lexer::getName() const throw()
{
    return this->actualSlice;
}

int Lexer::getValue() const throw()
{
    return atoi(this->actualSlice);
}

void Lexer::throwError(char* msg, SliceType e) const throw(std::runtime_error)
{
    std::stringstream sstm;
    sstm << "Exception in line " << this->actualLine;

    if (e != Unknown)
        if (e == Identifier || e == Number)
            sstm << ", in token \"" << this->actualSlice << "\"";
        else
            sstm << ", in token \"" << Lexer::reserved[e] << "\"";
    sstm << " : " << msg;

    throw std::runtime_error(sstm.str());
}
