#ifndef NOOP_PARSER_H
#define NOOP_PARSER_H

#include <noop.h>
#include <noop_type.h>
#include <vector>

namespace noop {
namespace Parser {
  extern std::vector<TokenType> Token;
  static String identifier_str;
  static String code_data;
  static DataNode* temp_data;
  extern int current_token;
  static int statement_token;
  extern std::vector<Statement*> body;

  void InitData(String& data);
  int GetToken();
  void HandleVarDeclaration();
  int GetNextToken();
  void ParseSpace(String&, int&);
  VarDeclarationStatement* ParserVar();
} // namespace Parser
} // namespace noop
#endif // NOOP_PARSER_H
