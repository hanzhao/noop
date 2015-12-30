#include <noop_parser.h>
#include <noop.h>
#include <noop_io.h>
#include <noop_type.h>

#include <string>
#include <iostream>

using namespace std;
using namespace noop;

namespace noop {
namespace Parser {
  void InitData(u16string& data) {
    code_data = data;
    std::vector<TokenType> Token;
    if (Token.size() == 0) {
      noop::InitToken();
    }
  }

  int GetToken() {
    static int last_char_pos = 0;
    ParseSpace(code_data, last_char_pos);
    if (isalpha(code_data[last_char_pos])) {
      identifier_str = code_data[last_char_pos];
      while (isalnum((code_data[++last_char_pos]))) {
        identifier_str += code_data[last_char_pos];
      }
    }
    DEBUG << code_data << endl;
    if (identifier_str == noop::Token[TOKEN::VAR_DECLARATION].name) {
      DEBUG << identifier_str << endl;
      return 1;
    }
    if (code_data[last_char_pos] == '\"') {
      val = u"";
      while (true) {
        if (code_data[last_char_pos - 1] == '\\' &&
            code_data[last_char_pos] == '\"') {
          break;
        }
        val += code_data[last_char_pos];
        last_char_pos++;
      }
    }
    return 0;
  }
  /*
  void Test() {
    GetNextToken();
    while (true) {
      if (current_token == Token::VAR_DECLARATION) {
        HandleVarDeclaration();
      } else {
        HandleTopLevelExpression();
      }
    }
  }
  */

  void ParseSpace(u16string& data, int& current_pos) {
    while (isspace(data[current_pos])) {
      ++current_pos;
    }
  }
} // namespace Parser
} // namespace noop
