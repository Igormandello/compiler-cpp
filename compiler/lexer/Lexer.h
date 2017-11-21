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
	void throwError(char* msg);
  private:
    FILE* file;
    char* actualSlice;
    int actualLine;
};

#endif
