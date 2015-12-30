#include <noop_type.h>
#include <vector>
#include <noop_io.h>
#include <noop_parser.h>

using namespace noop;
using namespace std;
namespace noop {
  std::vector<TokenType> Token;
  void InitToken() {
    Token.push_back(TokenType(u"EOS", "", 0));
    Token.push_back(TokenType(u"(", "", 0));
    Token.push_back(TokenType(u")", "", 0));
    Token.push_back(TokenType(u"[", "", 0));
    Token.push_back(TokenType(u"]", "", 0));
    Token.push_back(TokenType(u"{", "", 0));
    Token.push_back(TokenType(u"}", "", 0));
    Token.push_back(TokenType(u".", "", 0));
    Token.push_back(TokenType(u",", "", 0));
    Token.push_back(TokenType(u";", "", 0)); 
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
  Node* VarDeclarationStatement::Execute() {
    return new Node();
  };
}
