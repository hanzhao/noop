#ifndef NOOP_PARSER_H
#define NOOP_PARSER_H

#include <string>
#include <noop_type.h>
#include <vector>

using namespace std;

namespace noop {
namespace Parser {
  u16string identifier_str;
  u16string val;
  u16string code_data;
  Node* temp_data;
  int current_token;
  int statement_token;
  std::vector<Statement*> body;

  void InitData(u16string& data);
  int GetToken();
  void ParseSpace(u16string&, int&);
  void HandleVarDeclaration();
  int GetNextToken();
} // namespace Parser
} // namespace noop
#endif // NOOP_PARSER_H
