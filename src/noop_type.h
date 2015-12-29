#ifndef NOOP_TYPE_H
#define NOOP_TYPE_H

#include <string>

namespace noop {

enum StatementType {
  VAR_DECLARATION, // var x[ = ...];
  IF_ELSE, // if ... else ...
  WHILE, // while ...
  EXPRESSION // just a single expression
}

enum ExpressionType {
  BINARY, /* +, -, *, /, % */
  ASSIGNMENT, /* = */
  LOGIC, /* ==, !=, ===, !==, >=, <=, >, <, &&, || */
  BRACKETS, /* () */
  FUNCTION_CALL, /* func() */
  FUNCTION_DECLARATION /* function func(...arguments) { body } */
}

/* AST NodeType */
class Node {
public:
  virtual bool Execute(NoopScope& scope, NoopObject& ret) = 0;
}

class Body: Node {
private:
  std::vector<Statement> statements;
}

class Statement: public Node {
private:
  StatementType type;
public:
  virtual bool Execute(NoopScope& scope, NoopObject& ret) {
    /* Do nothing */
  }
}

/* var a = ..., b = ..., c; */
/* Expression is Atom 'undefined' when ```var c;``` */
class VarDeclarationStatement: public Statement {
private:
  std::vector<std::pair<std::u16string, Expression>> vars;
public:
  bool Execute(NoopScope& scope, NoopObject& ret) {

  }
}

/* if (cond) { if_body } else { else_body } */
class IfElseStatement: public Statement {
private:
  Expression cond;
  Body if_body, else_body;
}

/* while (cond) { body } */
class WhileStatement: public Statement {
private:
  Expression cond;
  Body body;
}

class Expression: public Statement {

}

/* +, -, *, /, % */
class BinaryExpression: public Expression {
private:
  Expression lhs, rhs;
}

/* = */
class AssignmentExpression: public Expression {
private:
  Expression lhs, rhs;
}

/* ==, !=, ===, !==, <=, >=, <, >, &&, || */
class LogicExpression: public Expression {
private:
  Expression lhs, rhs;
}

/* () */
class BracketsExpression: public Expression {
private:
  Expression inside;
}

/* func() */
class FunctionCallExpression: public Expression {
private:
  Expression func;
  std::vector<Expression> arguments;
}

/* function func(...arguments) { body } */
class FunctionDeclarationExpression: public Expression {
private:
  std::u16string func_name; /* Empty when it's anonymous */
  std::vector<std::u16string> arguments;
  Body body;
}

} // namespace noop

#endif // NOOP_TYPE_H
