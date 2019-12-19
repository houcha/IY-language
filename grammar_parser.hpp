#ifndef GRAMMAR_PARSER_HPP
#define GRAMMAR_PARSER_HPP


class GrammarParser {

  private:

    const char* pos_;

  public:

    GrammarParser(const char* text);

    int GetValue();

  private:

    int GetG();
    int GetE();
    int GetT();
    int GetP();
    int GetN();
};


#endif // GRAMMAR_PARSER_HPP

