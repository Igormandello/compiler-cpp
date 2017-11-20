#ifndef _LEXER_INCLUDED_

#define _LEXER_INCLUDED_
#include <stdio.h>
#include "SliceType.h"

class Lexer
{
  public:
    Lexer();
	Lexer(char* fileName);
	SliceType nextSlice(bool consume);
	bool hasMoreSlices();
	char* getName();
	int getValue();
  private:
    FILE* file;
    char* actualSlice;
};

#endif
