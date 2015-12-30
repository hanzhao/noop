#ifndef NOOP_PARSER_H
#define NOOP_PARSER_H

#include <string>
#include <noop_type.h>

namespace noop {
namespace Parser {
  extern std::vector<TokenType> Token;
  static std::u16string identifier_str;
  static std::u16string val;
  static bool is_number;
  static std::u16string code_data;
  static int current_token;

  void InitData(std::u16string& data);
  int GetToken();
  void ParseSpace(std::u16string&, int&);
} // namespace Parser
} // namespace noop
#endif // NOOP_PARSER_H
