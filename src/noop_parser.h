#ifndef NOOP_PARSER_H
#define NOOP_PARSER_H

#include <noop.h>

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
  VariableDeclaratorNode,
  VariableStatementNode,
  BodyNode,
  ProgramNode
};

} // namespace SyntaxTreeNodeType

/* Just an interface */
struct SyntaxTreeNode {
  int type;
};

/* SyntaxTreeNode */

struct Expression: SyntaxTreeNode {
  /* TODO */
};

/* Just an interface */
struct Statement: SyntaxTreeNode {

};

struct VariableDeclarator: SyntaxTreeNode {
  IdentifierToken* id;
  Expression* init;
  VariableDeclarator() {
    type = SyntaxTreeNodeType::VariableDeclaratorNode;
  }
};

struct VariableStatement: Statement {
  std::vector<VariableDeclarator*> declarations;
  String kind;
  VariableStatement() {
    type = SyntaxTreeNodeType::VariableStatementNode;
  }
};

struct ExpressionStatement: Statement {
  /* TODO */
};

struct Body: Statement {
  std::vector<Statement*> statements;
  Body() {
    type = SyntaxTreeNodeType::BodyNode;
  }
};

struct Program: SyntaxTreeNode {
  Body* body;
  Program() {
    type = SyntaxTreeNodeType::ProgramNode;
  }
};

/* SyntaxTreeResolver */
class SyntaxTree {
public:
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
  bool IsSamePunctuation(const String& value);
  void SkipUselessness();
  void GetKeyword(const String& keyword);
  void GetSemicolon();
  StringLiteralToken* GetStringLiteral();
  NumericLiteralToken* GetNumericLiteral();
  IdentifierToken* GetIdentifier();
  PunctuatorToken* GetPunctuator();
  VariableDeclarator* ParseVariableDeclarator();
  std::vector<VariableDeclarator*> ParseVariableDeclarationList(const String& kind);
  VariableStatement* ParseVariableStatement();
  Statement* ParseStatement();
  Body* ParseBody();
  Program* ParseProgram(String code);
};

extern SyntaxTree delegate;

} // namespace noop

#endif // NOOP_PARSER_H
