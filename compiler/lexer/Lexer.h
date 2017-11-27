#ifndef _LEXER_INCLUDED_

#define _LEXER_INCLUDED_
#include <stdio.h>
#include <stdexcept>
#include "SliceType.h"

class Lexer
{
  public:
	Lexer(char*) throw(std::invalid_argument);
	~Lexer() throw();
	SliceType nextSlice(bool) throw();
	bool hasMoreSlices() const throw();
	char* getName() const throw();
	int getValue() const throw();
	void throwError(char*, SliceType) const throw(std::runtime_error);
  private:
    FILE* file;
    static char* reserved[];
    static char  symbols[];
    char* actualSlice;
    int actualLine;
};

#endif
