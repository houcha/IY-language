#include "grammar_parser.hpp"
#include <cassert>
#include <cstdio>


#define ASSERT(condition, ...) {\
  if (!(condition)) {\
    printf("Error: " __VA_ARGS__);\
    fflush(stdout);\
    assert(condition);\
  }\
}

GrammarParser::GrammarParser(const char* text)
    : pos_(text) {}


int GrammarParser::GetValue() {
  return GetG();
}


int GrammarParser::GetG() {
  int val = GetE();
  ASSERT(*pos_ == '#', "expected #, got '%c' instead\n", *pos_);
  return val;
}

int GrammarParser::GetE() {
  int val = GetN();
  while (*pos_ == '-' || *pos_ == '+') {
    const char* prev_pos = pos_++;
    int next_val = GetN();
    if (*prev_pos == '-') {
      val -= next_val;
    } else {
      val += next_val;
    }
  }
  return val;
}

int GrammarParser::GetN() {
  int val = 0;
  do {
    ASSERT('0' <= *pos_ && *pos_ <= '9',
        "expected [0-9], got '%c' instead\n", *pos_);
    val = val*10 + *pos_ - '0';
    ++pos_;
  } while ('0' <= *pos_ && *pos_ <= '9');
  return val;
}

