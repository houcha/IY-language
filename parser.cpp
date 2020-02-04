#include "parser.hpp"
#include "node/operators.hpp"
#include "node/const_node.hpp"
#include "node/var_node.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>


#define ASSERT(condition, ...) {\
  if (!(condition)) {\
    printf("Error: " __VA_ARGS__);\
    printf("%s", pos_);\
    fflush(stdout);\
    assert(condition);\
  }\
}

bool IsDigit(char ch) {
  return '0' <= ch && ch <= '9';
}

Parser::Parser(const char* text)
    : pos_(text) {}

Node* Parser::GetTree() {
  return GetAll();
}

void Parser::SkipSpaces() {
  while (*pos_ == ' ' || *pos_ == '\t' || *pos_ == '\n') {
    pos_++;
  }
}

bool Parser::CurrentCharIs(char ch) {
  SkipSpaces();
  if (*pos_ == ch) {
    return true;
  }
  return false;
}

Node* Parser::GetAll() {
  BaseNode* result = new BaseNode();
  Node* decl = nullptr;
  while ((decl = GetVarDecl())  != nullptr ||
         (decl = GetFuncDecl()) != nullptr) {
    result->AddDecl(decl);
  }
  return result;
}

Node* Parser::GetFuncDecl() {
  Node* result = nullptr;
  Node* id = GetIdentifier();
  if (id == nullptr) {
    return nullptr;
  }
  if (CurrentCharIs('(')) {
    pos_++;
    Node* args = GetFuncDeclArgs();
    ASSERT(CurrentCharIs(')'),
        "expected ')' after '<func_decl_args>' in '<func_decl>'\n")
    pos_++;
    if (CurrentCharIs('{')) {
      pos_++;
      StatementNode* statements = new StatementNode();
      Node* next_statement = nullptr;
      while ((next_statement = GetStatement()) != nullptr) {
        pos_++;
        statements->AddStatement(next_statement);
      }
      ASSERT(CurrentCharIs('}'),
          "expected '}' after '{<statement>}*' in '<func_decl>'\n");
      pos_++;
      result = new FuncDeclNode(id->GetString(), args, statements);
    } else {
      delete args;
    }
  }
  delete id;
  return result;
}

Node* Parser::GetFuncDeclArgs() {
  const char* begin = pos_;
  ArgsNode* result = nullptr;
  Node* id = GetIdentifier();
  if (id != nullptr) {
    result = new ArgsNode();
    result->AddArg(id);
    while (CurrentCharIs(',')) {
      pos_++;
      Node* id = GetIdentifier();
      ASSERT(id != nullptr,
          "expected '<identifier>' after ',' in '<func_decl_args>'\n");
      result->AddArg(id);
    }
  } else { // Since the pattern is {...}?
    pos_ = begin;
  }
  return result;
}

Node* Parser::GetStatement() {
  Node* result = nullptr;
  const char* begin = pos_;
  result = GetCondition();
  if (result == nullptr) {
    pos_ = begin;
    result = GetLoop();
    if (result == nullptr) {
      pos_ = begin;
      result = GetVarDecl();
      if (result == nullptr) {
        pos_ = begin;
        result = GetAssignement();
        if (result == nullptr) {
          pos_ = begin;
          result = GetExpression();
          if (result == nullptr) {
            return nullptr;
          }
        }
        ASSERT(CurrentCharIs(';'), "expected ';' after '<statement>'\n");
        pos_++;
      }
    }
  }
  return result;
}

Node* Parser::GetVarDecl() {
  VarsNode* result = nullptr;
  Node* id = GetIdentifier();
  if (id == nullptr || strcmp(id->GetString().data(), "var") != 0) {
    delete id;
    return nullptr;
  }
  delete id;
  result = new VarsNode();
  Node* var = GetVariable();
  ASSERT(var != nullptr,
      "expected '<variable>' after 'var' in '<var_decl>'\n");
  result->AddVar(var);
  while (CurrentCharIs(',')) {
    pos_++;
    var = GetVariable();
    ASSERT(var != nullptr,
        "expected '<variable>' after ',' in '<var_decl>'\n")
    result->AddVar(var);
  }
  ASSERT(CurrentCharIs(';'), "expected ';' or ',' after '<variable>'\n");
  pos_++;
  return result;
}

Node* Parser::GetCondition() {
  Node* result = nullptr;
  Node* id = GetIdentifier();
  if (id == nullptr || strcmp(id->GetString().data(), "if") != 0) {
    delete id;
    return nullptr;
  }
  delete id;
  ASSERT(CurrentCharIs('('),
      "expected '(' after 'if' in '<condition>'\n");
  pos_++;
  Node* condition = GetExpression();
  ASSERT(condition != nullptr,
      "incorrect expression in if condition\n");
  ASSERT(CurrentCharIs(')'),
      "expected ')' after '<expression>' in '<condition>'\n");
  pos_++;
  ASSERT(CurrentCharIs('{'),
      "expected '{' after '(<expression>)' in '<condition>'\n");
  pos_++;
  StatementNode* statements = new StatementNode();
  Node* next_statement = nullptr;
  while ((next_statement = GetStatement()) != nullptr) { ////////// Does it correspond to grammar?
    statements->AddStatement(next_statement);
  }
  ASSERT(CurrentCharIs('}'),
      "expected '}' after '{{ {<statement>}*' in '<condition>'\n");
  pos_++;
  result = new IfNode(condition, statements);
  return result;
}

Node* Parser::GetLoop() {
  Node* result = nullptr;
  Node* id = GetIdentifier();
  if (id == nullptr || strcmp(id->GetString().data(), "while") != 0) {
    delete id;
    return nullptr;
  }
  delete id;
  ASSERT(CurrentCharIs('('),
      "expected '(' after 'while' in '<loop>'\n");
  pos_++;
  Node* condition = GetExpression();
  ASSERT(condition != nullptr,
      "incorrect expression in loop condition\n");
  ASSERT(CurrentCharIs(')'),
      "expected ')' after '<expression>' in '<loop>'\n");
  pos_++;
  ASSERT(CurrentCharIs('{'),
      "expected '{' after '(<expression>)' in '<loop>'\n");
  pos_++;
  StatementNode* statements = new StatementNode();
  Node* next_statement = nullptr;
  while ((next_statement = GetStatement()) != nullptr) {
    statements->AddStatement(next_statement);
  }
  ASSERT(CurrentCharIs('}'),
      "expected '}' after '{{ {<statement>}*' in '<loop>'\n");
  pos_++;
  result = new LoopNode(condition, statements);
  return result;
}

Node* Parser::GetAssignement() {
  Node* result = GetVariable();
  if (result != nullptr) {
    if (CurrentCharIs('=')) {
      pos_++;
      Node* expr = GetExpression();
      ASSERT(expr != nullptr,
          "expected '<expression>' after '=' in '<assignement>'\n")
      result = new AssignNode(result, expr);
    } else {
      delete result;
      result = nullptr;
    }
  }
  return result;
}

Node* Parser::GetExpression() {
  Node* result = GetMultiplication();
  if (result != nullptr) {
    while (CurrentCharIs('+') || CurrentCharIs('-')) {
      char op = *pos_;
      pos_++;
      Node* mul = GetMultiplication();
      ASSERT(mul != nullptr,
          "expected '<multiplication>' after '+' or '-' in '<expression>'\n")
      if (op == '+') {
        result = *result + *mul;
      } else {
        result = *result - *mul;
      }
    }
  }
  return result;
}

Node* Parser::GetMultiplication() {
  Node* result = GetUnitExpr();
  if (result != nullptr) {
    while (CurrentCharIs('*') || CurrentCharIs('/')) {
      char op = *pos_;
      pos_++;
      Node* unit = GetUnitExpr();
      ASSERT(unit != nullptr,
          "expected '<unit_expr>' after '*' or '/' in '<multiplication>'\n")
      if (op == '*') {
        result = *result * *unit;
      } else {
        result = *result / *unit;
      }
    }
  }
  return result;
}

Node* Parser::GetUnitExpr() {
  Node* result = nullptr;
  const char* begin = pos_;
  if (CurrentCharIs('(')) {
    pos_++;
    Node* expression = GetExpression();
    ASSERT(CurrentCharIs(')'),
        "expected ')' after '<expression>' in '<unit_expr>'\n")
    pos_++;
    result = expression;
  } else {
    result = GetNumber();
    if (result == nullptr) { // If not a number.
      pos_ = begin;
      result = GetFuncCall();
      if (result == nullptr) { // If not a function call.
        pos_ = begin;
        result = GetVariable();
      }
    }
  }
  return result;
}

Node* Parser::GetFuncCall() {
  Node* result = nullptr;
  Node* id = GetIdentifier();
  if (id == nullptr) { // If failed on identifier.
    return nullptr;
  }
  if (CurrentCharIs('(')) {
    pos_++;
    Node* args = GetCallArgs();
    ASSERT(CurrentCharIs(')'),
        "expected ')' after '<call_args>' in '<func_call>'\n")
    pos_++;
    result = new FuncCallNode(id->GetString(), args);
  }
  delete id;
  return result;
}

Node* Parser::GetCallArgs() {
  const char* begin = pos_;
  ArgsNode* result = nullptr;
  Node* expr = GetExpression();
  if (expr != nullptr) {
    result = new ArgsNode();
    result->AddArg(expr);
    while (CurrentCharIs(',')) {
      pos_++;
      Node* next_expr = GetExpression();
      ASSERT(next_expr != nullptr,
          "expected '<expression>' after ',' in '<call_args>'\n")
      result->AddArg(next_expr);
    }
  } else { // Since the pattern is {...}?
    pos_ = begin;
  }
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

Node* Parser::GetNumber() {
  SkipSpaces();
  int val = 0;
  do {
    if (!IsDigit(*pos_)) {
      return nullptr;
    }
    val = val*10 + *pos_ - '0';
    ++pos_;
  } while (IsDigit(*pos_));
  return new ConstNode<int>(val);
}

Node* Parser::GetIdentifier() {
  SkipSpaces();
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

