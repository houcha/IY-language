#ifndef EXPRESSION_PARSER_HPP
#define EXPRESSION_PARSER_HPP


class ExpressionParser {

  private:

    const char* pos_;

  public:

    ExpressionParser(const char* text);

    int GetExpression();

  private:

    int GetG();
    int GetE();
    int GetT();
    int GetP();
    int GetN();
};


#endif // EXPRESSION_PARSER_HPP

