#ifndef EXPRESSION_PARSER_HPP
#define EXPRESSION_PARSER_HPP

#include "node/node.hpp"
#include "node/math_node.hpp"
#include "node/const_node.hpp"
#include "node/operators.hpp"
#include <stack>
#include <cstring>


class Parser {

    const char* pos_;

  public:

    Parser(const char* text);

    Node* GetTree();

  private:

    Node* GetAll();
    Node* GetFuncDecl();
    Node* GetFuncDeclArgs();
    Node* GetStatement();
    Node* GetVarDecl();
    Node* GetCondition();
    Node* GetLoop();
    Node* GetAssignement();
    Node* GetExpression();
    Node* GetMultiplication();
    Node* GetUnitExpr();
    Node* GetNumber();
    Node* GetFuncCall();
    Node* GetCallArgs();
    Node* GetVariable();
    Node* GetIdentifier();

    bool CurrentCharIs(char ch);
    void SkipSpaces();
};


#endif // EXPRESSION_PARSER_HPP

