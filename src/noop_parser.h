#ifndef NOOP_PARSER_H
#define NOOP_PARSER_H

#include <noop.h>
#include <noop_context.h>

#include <vector>

namespace noop {

namespace TokenType {

enum {
  EndOfSource, /* End of source code */
  Keyword,
  Identifier,
  BooleanLiteral,
  NullLiteral,
  NumberLiteral,
  StringLiteral,
  Punctuator
};

} // namespace TokenType

struct Token {
  int type, start, end;
  Token(int type, int start, int end) : type(type), start(start), end(end) {

  }
};

struct StringLiteralToken: Token {
  String value;
  StringLiteralToken(int type, int start, int end): Token(type, start, end) {
    value = U"";
  }
};

struct IdentifierToken: Token {
  String name;
  IdentifierToken(int type, int start, int end): Token(type, start, end)  {
    name = U"";
  }
};

struct PunctuatorToken: Token {
  String value;
  PunctuatorToken(int type, int start, int end): Token(type, start, end)  {
    value = U"";
  }
};

struct NumericLiteralToken: Token {
  Number value;
  NumericLiteralToken(int type, int start, int end): Token(type, start, end)  {
    value = 0.0;
  }
};

namespace SyntaxTreeNodeType {

enum {
  NullLiteral,
  BooleanLiteral,
  StringLiteral,
  NumberLiteral,
  Identifier,
  ThisExpression,
  MemberExpression,
  CallExpression,
  AssignmentExpression,
  SequenceExpression,
  BinaryExpression,
  VariableDeclarator,
  VariableStatement,
  ExpressionStatement,
  Body,
  Program
};

} // namespace SyntaxTreeNodeType

/* Just an interface */
struct SyntaxTreeNode {
  int type;
  virtual bool Execute() = 0;
};

/* SyntaxTreeNode */

struct Expression: SyntaxTreeNode {
  virtual bool Execute() = 0;
};

struct Literal: Expression {
  /* Just an interface */
};

struct Identifier: Expression {
  String name;
  Identifier() {
    type = SyntaxTreeNodeType::Identifier;
  }
};

struct ThisExpression: Expression {
  ThisExpression() {
    type = SyntaxTreeNodeType::ThisExpression;
  }
};

struct MemberExpression: Expression {
  String _operator;
  Expression* left;
  Expression* right;
  MemberExpression() {
    type = SyntaxTreeNodeType::MemberExpression;
  }
};

struct CallExpression: Expression {
  Expression* callee;
  std::vector<Expression*> arguments;
  CallExpression() {
    type = SyntaxTreeNodeType::CallExpression;
  }
};

struct AssignmentExpression: Expression {
  String _operator;
  Expression* left;
  Expression* right;
  AssignmentExpression() {
    type = SyntaxTreeNodeType::AssignmentExpression;
  }
};

struct SequenceExpression: Expression {
  std::vector<Expression *> expressions;
  SequenceExpression() {
    type = SyntaxTreeNodeType::SequenceExpression;
  }
};

struct BinaryExpression: Expression {
  String _operator;
  Expression* left;
  Expression* right;
  BinaryExpression() {
    type = SyntaxTreeNodeType::BinaryExpression;
  }
};

struct NumberLiteral: Literal {
  Number value;
  NumberLiteral() {
    type = SyntaxTreeNodeType::NumberLiteral;
  }
};

struct StringLiteral: Literal {
  String value;
  StringLiteral() {
    type = SyntaxTreeNodeType::StringLiteral;
  }
};

struct BooleanLiteral: Literal {
  bool value;
  BooleanLiteral() {
    type = SyntaxTreeNodeType::BooleanLiteral;
  }
};

struct NullLiteral: Literal {
  NullLiteral() {
    type = SyntaxTreeNodeType::NullLiteral;
  }
};

/* Just an interface */
struct Statement: SyntaxTreeNode {
  virtual bool Execute() = 0;
};

struct VariableDeclarator: SyntaxTreeNode {
  IdentifierToken* id;
  Expression* init;
  VariableDeclarator() {
    type = SyntaxTreeNodeType::VariableDeclarator;
  }
  virtual bool Execute();
};

struct VariableStatement: Statement {
  std::vector<VariableDeclarator*> declarations;
  String kind;
  VariableStatement() {
    type = SyntaxTreeNodeType::VariableStatement;
  }
  virtual bool Execute();
};

struct ExpressionStatement: Statement {
<<<<<<< 631529c222996333910f82a35f373521fc9116f8
  Expression* expression;
  ExpressionStatement() {
    type = SyntaxTreeNodeType::ExpressionStatement;
  }
=======
  virtual bool Execute();
  /* TODO */
>>>>>>> finish declaration
};

struct Body: Statement {
  std::vector<Statement*> statements;
  Body() {
    type = SyntaxTreeNodeType::Body;
  }
  virtual bool Execute();
};

struct Program: SyntaxTreeNode {
  Body* body;
  Program() {
    type = SyntaxTreeNodeType::Program;
  }
  virtual bool Execute();
};

/* SyntaxTreeResolver */
class SyntaxTree {
public:
  Literal* CreateLiteral(Token* token);
  Identifier* CreateIdentifier(Token* token);
  ThisExpression* CreateThisExpression();
  MemberExpression* CreateMemberExpression(String op, Expression* expr, Expression* property);
  CallExpression* CreateCallExpression(Expression* expr, std::vector<Expression*> args);
  AssignmentExpression* CreateAssignmentExpression(String op, Expression* left, Expression* right);
  SequenceExpression* CreateSequenceExpression(std::vector<Expression*> expressions);
  BinaryExpression* CreateBinaryExpression(String op, Expression* left, Expression* right);
  VariableDeclarator* CreateVariableDeclarator(IdentifierToken* id,
                                               Expression* init);
  VariableStatement* CreateVariableStatement(String kind,
                              std::vector<VariableDeclarator*> declarations);
  ExpressionStatement* CreateExpressionStatement(Expression* expr);
  Program* CreateProgram(Body* body);
};

class Parser {
private:
  String source;
  int index;
  int length;
  Token* look_ahead;
public:
  Token* LookAhead();
  void Peek();
  Token* Lex();
  bool IsPunctuation(const String& value);
  bool IsKeyword(const String& value);
  bool IsIdentifierName(Token* token);
  void SkipUselessness();
  void GetKeyword(const String& keyword);
  void GetSemicolon();
  StringLiteralToken* GetStringLiteral();
  NumericLiteralToken* GetNumericLiteral();
  IdentifierToken* GetIdentifier();
  PunctuatorToken* GetPunctuator();
  VariableDeclarator* ParseVariableDeclarator();
  Expression* ParsePrimaryExpression();
  Identifier* ParseLiteralProperty();
  Expression* ParseComputedProperty();
  std::vector<Expression*> ParseArguments();
  Expression* ParsePostfixExpression();
  Expression* ParseBinaryExpression();
  Expression* ParseAssignmentExpression();
  Expression* ParseExpression();
  std::vector<VariableDeclarator*> ParseVariableDeclarationList();
  VariableStatement* ParseVariableStatement();
  Statement* ParseStatement();
  Body* ParseBody();
  Program* ParseProgram(String code);
};

extern SyntaxTree delegate;
} // namespace noop

#endif // NOOP_PARSER_H
