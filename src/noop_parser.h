#ifndef NOOP_PARSER_H
#define NOOP_PARSER_H

#include <string>
#include <noop_type.h>
#include <vector>

namespace noop {
namespace Parser {
  extern std::vector<TokenType> Token;
  static std::u16string identifier_str;
  static std::u16string code_data;
  static DataNode* temp_data;
  extern int current_token;
  static int statement_token;
  extern std::vector<Statement*> body;

  void InitData(std::u16string& data);
  int GetToken();
  void HandleVarDeclaration();
  int GetNextToken();
  void ParseSpace(std::u16string&, int&);
  VarDeclarationStatement* ParserVar();
} // namespace Parser
} // namespace noop
#endif // NOOP_PARSER_H
