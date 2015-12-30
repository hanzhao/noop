#ifndef NOOP_PARSER_H
#define NOOP_PARSER_H

#include <string>
#include <noop_type.h>

using namespace std;

namespace noop {
namespace Parser {
  static u16string identifier_str;
  static u16string val;
  static bool is_number;
  static u16string code_data;
  static int current_token;

  void InitData(u16string& data);
  int GetToken();
  void ParseSpace(u16string&, int&);
} // namespace Parser
} // namespace noop
#endif // NOOP_PARSER_H
