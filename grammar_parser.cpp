#include "grammar_parser.hpp"
#include <cassert>


GrammarParser::GrammarParser(const char* text)
    : pos_(text) {}


int GrammarParser::GetValue() {
  GetG();
}


int GrammarParser::GetG() {
  int val = GetN();
  assert(*pos_ == '#');
  return val;
}

int GrammarParser::GetN() {
  int val = 0;
  if ('0' <= *pos_ && *pos_ <= '9') {
    val = *pos_ - '0';
    ++pos_;
  }
  return val;
}

