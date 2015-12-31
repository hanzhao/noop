#ifndef NOOP_TYPE_H
#define NOOP_TYPE_H

#include <noop.h>
#include <vector>

namespace noop {

enum TOKEN {
  /* End of source indicator. */
  EOS,    // EOS

  /* Punctuators */
  LPAREN,     // (
  RPAREN,     // )
  LBRACK,     // [
  RBRACK,     // ]
  LBRACE,     // {
  RBRACE,     // }
  PERIOD,     // .
  COMMA,      // ,
  SEMICOLON,  // ;

  /* Statement */
  VAR_DECLARATION,        // var x[ = ...];
  IF,                     // if ...
  ELSE,                   // else ...
  WHILE,                  // while ...
  EXPRESSION,             // just a single expression
  FUNCTION_CALL,          /* func() */
  FUNCTION_DECLARATION,   /* function func(...arguments) { body } */

  /* Binary Operation */
  ADD,        // +
  SUB,        // -
  MUL,        // *
  DIV,        // /
  ASSIGN,     // =
  EQ,         // ==
  NE,         // !=
  EQ_STRICT,  // ===
  NE_STRICT,  // !==
  GT,         // >
  LT,         // <
  GTE,        // >=
  LTE,        // <=
  AND,        // &&
  OR,         // ||
  INIT,
  NUMBER,     // [0-9.]+
  BOOL,       // true | false
  STRING,       // [a-zA-Z][a-zA-Z0-9]*
  DATA,
  BLANK,
};

class TokenType {
public:
  String name;
  TOKEN type;
  int prec;
  TokenType(String _name, TOKEN _type, int _prec = 0)
    : name(_name), type(_type), prec(_prec) {};
};

extern std::vector<TokenType> Token;

/* AST NodeType */
class Node {
public:
  TOKEN type;
  virtual TOKEN GetType() const { return type; };
  virtual ~Node() {};
};

class Statement: public Node {
public:
  virtual Node* Execute() = 0;
  virtual void Print() = 0;
};

class DataNode: public Node {
public:
  TOKEN type_;
  virtual TOKEN GetType_() const { return type_; };
  DataNode() { type = TOKEN::DATA; };
  virtual void Print() = 0;
};

class NumberNode: public DataNode {
public:
  double val;
  NumberNode(double _val): val(_val) { type_ = TOKEN::NUMBER; };
  void Print();
};

class StringNode: public DataNode {
public:
  String val;
  StringNode(String _val): val(_val) { type_ = TOKEN::STRING; };
  void Print();
};

class VariableNode: public Node {
public:
  String name;
  VariableNode(const String& _name): name(_name) { type = TOKEN::STRING; };
};

class Expression: public Statement {
public:
  TOKEN type_;
  virtual TOKEN GetType_() const { return type_; };
  Expression() { type = TOKEN::EXPRESSION; };
};

class AtomExpr: public Expression {
public:
  DataNode* data;
  AtomExpr() { type_ = TOKEN::INIT; };
  AtomExpr(DataNode* _data): data(_data) { type_ = TOKEN::INIT; };
  Node* Execute();
  void Print();
};

class VarDeclarationStatement: public Statement {
public:
  std::vector<std::pair<String, Expression*> > vars;
  VarDeclarationStatement() { type = TOKEN::VAR_DECLARATION; };
  Node* Execute();
  void Print();
};

/*
// AST NodeType
class Node {
public:
  virtual bool Execute(NoopScope& scope, NoopObject& ret) = 0;
};

class Statement: public Node {
private:
  StatementType type;
public:
  virtual bool Execute(NoopScope& scope, NoopObject& ret) {
    // TODO
  }
};

class Body: Node {
private:
  std::vector<Statement> statements;
};

class NumberNode: Node {
  double val;
public:
  NumberNode(double _val): val(_val) {}
};

class StringNode: Node {
  String val;
public:
  StringNode(String _val): val(_val) {}
};

class Expression: public Statement {

};
// var a = ..., b = ..., c;
// Expression is Atom 'undefined' when ```var c;```
class VarDeclarationStatement: public Statement {
private:
  std::vector<std::pair<String, Expression>> vars;
public:
  bool Execute(NoopScope& scope, NoopObject& ret) {
    // TODO
  }
};

// if (cond) { if_body } else { else_body }
class IfElseStatement: public Statement {
private:
  Expression cond;
  Body if_body, else_body;
};

// while (cond) { body }
class WhileStatement: public Statement {
private:
  Expression cond;
  Body body;
};

// +, -, *, /, %
class BinaryExpression: public Expression {
private:
  Expression lhs, rhs;
};

// =
class AssignmentExpression: public Expression {
private:
  Expression lhs, rhs;
};

// ==, !=, ===, !==, <=, >=, <, >, &&, ||
class LogicExpression: public Expression {
private:
  Expression lhs, rhs;
};

// ()
class BracketsExpression: public Expression {
private:
  Expression inside;
};

// func()
class FunctionCallExpression: public Expression {
private:
  Expression func;
  std::vector<Expression> arguments;
};

// function func(...arguments) { body }
class FunctionDeclarationExpression: public Expression {
private:
  String func_name; // Empty when it's anonymous
  std::vector<String> arguments;
  Body body;
};
*/
void InitToken();
} // namespace noop


#endif // NOOP_TYPE_H
