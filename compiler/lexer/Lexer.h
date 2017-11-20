#include "SliceType.h"

class Lexer
{
  public:
	Lexer(char* fileName);
	SliceType nextSlice(bool consume);
	bool hasMoreSlices();
	char* getName();
	int getValue();
  private:
    FILE* file;
    char* actualSlice;
};
