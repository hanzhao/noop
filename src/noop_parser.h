#ifndef NOOP_PARSER_H
#define NOOP_PARSER_H

#include <string>
#include <noop_type.h>
#include <vector>

namespace noop {
namespace Parser {
  extern std::vector<TokenType> Token;
  std::u16string identifier_str;
  std::u16string val;
  std::u16string code_data;
  DataNode* temp_data;
  int current_token;
  int statement_token;
  std::vector<Statement*> body;

  void InitData(std::u16string& data);
  int GetToken();
  void HandleVarDeclaration();
  int GetNextToken();
  void ParseSpace(std::u16string&, int&);
} // namespace Parser
} // namespace noop
#endif // NOOP_PARSER_H
