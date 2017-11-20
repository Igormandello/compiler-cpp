class Parser
{
  public:
    Parser(Lexer* l);
    void compileProgramInit();
    void compileVariable();
    void compileProcedure();
    void compileFunction();
    void compileMain();
    void compileIf();
    void compileWhile();
};
