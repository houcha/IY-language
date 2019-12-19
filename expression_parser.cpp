#include "expression_parser.hpp"
#include <cassert>
#include <cstdio>


#define ASSERT(condition, ...) {\
  if (!(condition)) {\
    printf("Error: " __VA_ARGS__);\
    fflush(stdout);\
    assert(condition);\
  }\
}

ExpressionParser::ExpressionParser(const char* text)
    : pos_(text) {}


int ExpressionParser::GetExpression() {
  return GetG();
}


int ExpressionParser::GetG() {
  int val = GetE();
  ASSERT(*pos_ == '#', "expected #, got '%c' instead\n", *pos_);
  return val;
}

int ExpressionParser::GetE() {
  int val = GetT();
  while (*pos_ == '-' || *pos_ == '+') {
    char op = *pos_;
    pos_++;
    int next_val = GetT();
    if (op == '-') {
      val -= next_val;
    } else {
      val += next_val;
    }
  }
  return val;
}

int ExpressionParser::GetT() {
  int val = GetP();
  while (*pos_ == '*' || *pos_ == '/') {
    char op = *pos_;
    pos_++;
    int next_val = GetP();
    if (op == '*') {
      val *= next_val;
    } else {
      val /= next_val;
    }
  }
  return val;
}

int ExpressionParser::GetP() {
  int val = 0;
  if (*pos_ == '(') {
    pos_++;
    val = GetE();
    ASSERT(*pos_ == ')', "expected ')', got '%c' instead\n", *pos_);
    pos_++;
  } else {
    val = GetN();
  }
  return val;
}

int ExpressionParser::GetN() {
  int val = 0;
  do {
    ASSERT('0' <= *pos_ && *pos_ <= '9',
        "expected [0-9], got '%c' instead\n", *pos_);
    val = val*10 + *pos_ - '0';
    ++pos_;
  } while ('0' <= *pos_ && *pos_ <= '9');
  return val;
}

