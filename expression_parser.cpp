#include "expression_parser.hpp"
#include "node/operators.hpp"
#include "node/const_node.hpp"
#include "node/var_node.hpp"
#include <cassert>
#include <cstdio>


#define ASSERT(condition, ...) {\
  if (!(condition)) {\
    printf("Error: " __VA_ARGS__);\
    fflush(stdout);\
    assert(condition);\
  }\
}

bool IsDigit(char ch) {
  return '0' <= ch && ch <= '9';
}

Parser::Parser(const char* text)
    : pos_(text),
      error_() {}


Node* Parser::GetArgs() {
  const char* begin = pos_;
  ArgsNode* result = nullptr;
  Node* id = GetIdentifier();
  if (id != nullptr) {
    result = new ArgsNode();
    while (*pos_ == ',') {
      pos_++;
      Node* next_id = GetIdentifier();
      if (next_id != nullptr) {
        result->AddArg(next_id);
      } else {
        error_.PushError("expected '<identifier>' after ','\n");
        delete result;
        result = nullptr;
      }
    }
  } else { // Since {...}?
    pos_ = begin;
  }
  return result;
}

Node* Parser::GetExpression() {
  const char* begin = pos_;
  Node* result = GetVariable();
  if (result != nullptr) {
    if (*pos_ == '=') {
      pos_++;
      Node* expr = GetExpression();
      if (expr != nullptr) {
        result = new AssignNode(result, expr);
      } else {
        delete result;
        result = nullptr;
      }
    } else {
      delete result;
      result = nullptr;
    }
  }
  if (result == nullptr) {
    pos_ = begin;
    result = GetSum();
    if (result == nullptr) {
      error_.PushError("expected '<variable> = <expression>' or '<sum>'\n");
    }
  }
  return result;
}

Node* Parser::GetSum() {
  Node* result = GetMultiplication();
  if (result != nullptr) {
    while (*pos_ == '+' || *pos_ == '-') {
      char op = *pos_;
      pos_++;
      Node* mul = GetMultiplication();
      if (mul != nullptr) {
        if (op == '+') {
          result = *result + *mul;
        } else {
          result = *result - *mul;
        }
      } else {
        error_.PushError("expected '<multiplication>' after '+' or '-'\n");
        delete mul;
        delete result;
        result = nullptr;
        break;
      }
    }
  }
  return result;
}

Node* Parser::GetMultiplication() {
  Node* result = GetUnitExpr();
  if (result != nullptr) {
    while (*pos_ == '*' || *pos_ == '/') {
      char op = *pos_;
      pos_++;
      Node* unit = GetUnitExpr();
      if (unit != nullptr) {
        if (op == '*') {
          result = *result * *unit;
        } else {
          result = *result / *unit;
        }
      } else {
        error_.PushError("expected '<unit_expr>' after '*' or '/'\n");
        delete unit;
        delete result;
        result = nullptr;
        break;
      }
    }
  }
  return result;
}

Node* Parser::GetUnitExpr() {
  const char* begin = pos_;
  Node* result = nullptr;
  if (*pos_ == '(') {
    pos_++;
    Node* expression = GetExpression();
    if (*pos_ == ')') {
      pos_++;
      result = expression;
    } else {
      error_.PushError("expected ')' after '(<expression>'\n");
      delete expression;
    }
  } else {
    result = GetNumber();
    if (result == nullptr) { // Is not a number.
      pos_ = begin;
      result = GetFunctionCall();
      if (result == nullptr) { // If not a function call.
        pos_ = begin;
        result = GetVariable(); // It is nullptr if failed to read variable.
        if (result == nullptr) {
          error_.PushError("expected""'(<expression)',"
                                     "'<number>',"
                                     "'<function_call>' or"
                                     "'<variable>'\n");
        }
      }
    }
  }
  return result;
}

Node* Parser::GetNumber() {
  int val = 0;
  do {
    if (!IsDigit(*pos_)) { // If failed.
      error_.PushError("expected '[0-9]'\n");
      return nullptr;
    }
    val = val*10 + *pos_ - '0';
    ++pos_;
  } while (IsDigit(*pos_));
  return new ConstNode<int>(val);
}

Node* Parser::GetFunctionCall() {
  Node* result = nullptr;
  Node* id = GetIdentifier();
  if (id == nullptr) { // If failed on identifier.
    return nullptr;
  }
  if (*pos_ == '(') {
    pos_++;
    Node* args = GetArgs();
    if (*pos_ == ')') {
      pos_++;
      result = new FunctionCallNode(id->GetString(), args);
    } else { // If failed on ')'
      error_.PushError("expected ')' after function arguments\n");
      delete args;
    }
  } else { // If failed on '('
    error_.PushError("expected '(' after function identifier\n");
  }
  delete id;
  return result;
}

Node* Parser::GetVariable() {
  Node* result = nullptr;
  Node* id = GetIdentifier();
  if (id != nullptr) {
    result = new VarNode(id->GetString());
    delete id;
  }
  return result;
}

Node* Parser::GetIdentifier() {
  std::string name;
  do {
    if (!(('a' <= *pos_ && *pos_ <= 'z') || ('A' <= *pos_ && *pos_ <= 'Z'))) {
      return nullptr;
    }
    name.push_back(*pos_);
    pos_++;
  } while (('a' <= *pos_ && *pos_ <= 'z') || ('A' <= *pos_ && *pos_ <= 'Z'));
  return new IdentifierNode(name);
}

