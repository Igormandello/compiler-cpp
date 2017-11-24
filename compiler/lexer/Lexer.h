#ifndef _LEXER_INCLUDED_

#define _LEXER_INCLUDED_
#include <stdio.h>
#include "SliceType.h"

class Lexer
{
  public:
	Lexer(char* fileName);
	~Lexer();
	SliceType nextSlice(bool consume);
	bool hasMoreSlices();
	char* getName();
	int getValue();
	void throwError(char* msg);
  private:
    FILE* file;
    static char* reserved[];
    static char  symbols[];
    char* actualSlice;
    int actualLine;
};

#endif
