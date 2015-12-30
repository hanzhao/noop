#include <noop_type.h>
#include <vector>

using namespace noop;
namespace noop {
  void InitToken() {
    std::vector<TokenType> Token;
    Token.push_back(TokenType(u"EOS", "", 0));
    Token.push_back(TokenType(u"(", "", 0));
    Token.push_back(TokenType(u")", "", 0));
    Token.push_back(TokenType(u"[", "", 0));
    Token.push_back(TokenType(u"]", "", 0));
    Token.push_back(TokenType(u"{", "", 0));
    Token.push_back(TokenType(u"}", "", 0));
    Token.push_back(TokenType(u".", "", 0));
    Token.push_back(TokenType(u",", "", 0));
    Token.push_back(TokenType(u"var", "", 0));
    Token.push_back(TokenType(u"if", "", 0));
    Token.push_back(TokenType(u"else", "", 0));
    Token.push_back(TokenType(u"while", "", 0));
    Token.push_back(TokenType(u"expression", "", 0));
    Token.push_back(TokenType(u"function_call", "", 0));
    Token.push_back(TokenType(u"function_declaration", "", 0));
    Token.push_back(TokenType(u"+", "", 0));
    Token.push_back(TokenType(u"-", "", 0));
    Token.push_back(TokenType(u"*", "", 0));
    Token.push_back(TokenType(u"/", "", 0));
    Token.push_back(TokenType(u"=", "", 0));
    Token.push_back(TokenType(u"==", "", 0));
    Token.push_back(TokenType(u"!=", "", 0));
    Token.push_back(TokenType(u"===", "", 0));
    Token.push_back(TokenType(u"!==", "", 0));
    Token.push_back(TokenType(u">", "", 0));
    Token.push_back(TokenType(u"<", "", 0));
    Token.push_back(TokenType(u">=", "", 0));
    Token.push_back(TokenType(u"<=", "", 0));
    Token.push_back(TokenType(u"&&", "", 0));
    Token.push_back(TokenType(u"||", "", 0));
  }
}
