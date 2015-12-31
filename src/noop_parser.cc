#include <noop_parser.h>
#include <noop.h>
#include <noop_io.h>
#include <noop_type.h>

#include <string>
#include <iostream>

using namespace std;


namespace noop {
namespace Parser {

  vector<TokenType> Token;
  int current_token;
  std::vector<Statement*> body;

  void InitData(u16string& data) {
    code_data = data;
    if (Token.size() == 0) {
      InitToken();
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
      if (identifier_str == noop::Token[TOKEN::VAR_DECLARATION].name) {
        // DEBUG << "identifier_str:" << identifier_str << endl;
        return TOKEN::VAR_DECLARATION;
      }
      temp_data = new StringNode(identifier_str);
      return TOKEN::STRING;
    }

    if (code_data[last_char_pos] == '\"') {
      identifier_str = u"";
      int ahead_pos = last_char_pos;
      while (!(code_data[++last_char_pos] == '\"' &&
             code_data[ahead_pos] != '\\'))  {
        identifier_str += code_data[++ahead_pos];
      }
      ++last_char_pos;
      temp_data = new StringNode(identifier_str);
      return TOKEN::STRING;
    }

    if (code_data[last_char_pos] == '=') {
      ++last_char_pos;
      return TOKEN::ASSIGN;
    }

    if (code_data[last_char_pos] == ';') {
      ++last_char_pos;
      return TOKEN::SEMICOLON;
    }

    return TOKEN::BLANK;
  }

  int GetNextToken() {
    return current_token = GetToken();
  };

  void ParseSpace(u16string& data, int& current_pos) {
    while (isspace(data[current_pos])) {
      ++current_pos;
    }
  }

  VarDeclarationStatement* ParserVar() {
    VarDeclarationStatement* temp = new VarDeclarationStatement();
    int this_token;
    while (true) {
      this_token = GetToken();
      if (this_token == TOKEN::ASSIGN) {
        u16string this_name = identifier_str;
        GetToken();
        Expression* atom = new AtomExpr(temp_data);
        temp->vars.push_back(make_pair(this_name, atom));
      } else if (this_token == TOKEN::SEMICOLON) {
        break;
      }
    }
    return temp;
  }

  void HandleVarDeclaration() {
    body.push_back(ParserVar());
    GetNextToken();
  }
} // namespace Parser
} // namespace noop
