#include <iostream>
#include "SliceType.h"

class LexicalAnalyzer
{
  public:
	LexicalAnalyzer(char* fileName);
	SliceType nextSlice();
	char hasMoreSlices();
	char* getName();
	int getValue();
};
