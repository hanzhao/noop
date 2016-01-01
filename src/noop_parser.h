#ifndef NOOP_PARSER_H
#define NOOP_PARSER_H

#include <noop.h>
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

} // TokenType

class Token {
public:
  int type, start, end;
  Token(int type, int start, int end);
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
  Program* ParseProgram(String code);
};

} // namespace noop

#endif // NOOP_PARSER_H
