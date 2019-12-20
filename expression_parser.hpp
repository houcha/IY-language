#ifndef EXPRESSION_PARSER_HPP
#define EXPRESSION_PARSER_HPP

#include "node/node.hpp"
#include "node/math_node.hpp"
#include "node/const_node.hpp"
#include "node/operators.hpp"
#include <stack>
#include <cstring>


class ErrorHandler {

  private:

    static constexpr int ERROR_MAX_LEN = 100;
    std::stack<char*> stack_;

  public:

    ErrorHandler()
        : stack_() {}

    ~ErrorHandler() {
      while (!stack_.empty()) {
        free(stack_.top());
        stack_.pop();
      }
    }

    void PushError(const char* error) {
      char* heap_error = (char*)calloc(strlen(error), sizeof(char));
      stack_.push(heap_error);
    }
};

class Parser {

    const char* pos_;
    ErrorHandler error_;

  public:

    Parser(const char* text);

    Node* GetExpression();

  private:

    Node* GetArgs();
    Node* GetSum();
    Node* GetMultiplication();
    Node* GetUnitExpr();
    Node* GetNumber();
    Node* GetFunctionCall();
    Node* GetVariable();
    Node* GetIdentifier();
};


#endif // EXPRESSION_PARSER_HPP

