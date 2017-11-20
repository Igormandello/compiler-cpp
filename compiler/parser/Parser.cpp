#include "Parser.h"

Parser::Parser(Lexer l)
{
    this->lexer = l;
}

void Parser::compileMain()
{
  compileCompoundCommand();

  SliceType next = this->lexer.nextSlice(true);
  if (next != Point)
    throw "Missing point after main";
}

void Parser::compileCompoundCommand()
{

}
