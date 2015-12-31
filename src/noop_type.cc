#include <noop_type.h>
#include <vector>
#include <noop.h>
#include <noop_io.h>
#include <noop_parser.h>

using namespace noop;
using namespace std;

static std::string TokenToString(TOKEN type) {
  if (type == TOKEN::VAR_DECLARATION) {
    return "VAR_DECLARATION";
  } else if (type == TOKEN::EXPRESSION) {
    return "EXPRESSION";
  } else if (type == TOKEN::INIT) {
    return "INIT";
  } else if (type == TOKEN::STRING) {
    return "STRING";
  }{
    return "UNKNOWN";
  }
}
namespace noop {
  std::vector<TokenType> Token;
  void InitToken() {
    Token.push_back(TokenType(u"EOS", TOKEN::EOS));
    Token.push_back(TokenType(u"(", TOKEN::LPAREN));
    Token.push_back(TokenType(u")", TOKEN::RPAREN));
    Token.push_back(TokenType(u"[", TOKEN::LBRACK));
    Token.push_back(TokenType(u"]", TOKEN::RBRACK));
    Token.push_back(TokenType(u"{", TOKEN::LBRACE));
    Token.push_back(TokenType(u"}", TOKEN::RBRACE));
    Token.push_back(TokenType(u".", TOKEN::PERIOD));
    Token.push_back(TokenType(u",", TOKEN::COMMA));
    Token.push_back(TokenType(u";", TOKEN::SEMICOLON));
    Token.push_back(TokenType(u"var", TOKEN::VAR_DECLARATION));
    Token.push_back(TokenType(u"if", TOKEN::IF));
    Token.push_back(TokenType(u"else", TOKEN::ELSE));
    Token.push_back(TokenType(u"while", TOKEN::WHILE));
    Token.push_back(TokenType(u"expression", TOKEN::EXPRESSION));
    Token.push_back(TokenType(u"function_call", TOKEN::FUNCTION_CALL));
    Token.push_back(TokenType(u"function_declaration", TOKEN::FUNCTION_DECLARATION));
    Token.push_back(TokenType(u"+", TOKEN::ADD));
    Token.push_back(TokenType(u"-", TOKEN::SUB));
    Token.push_back(TokenType(u"*", TOKEN::MUL));
    Token.push_back(TokenType(u"/", TOKEN::DIV));
    Token.push_back(TokenType(u"=", TOKEN::ASSIGN));
    Token.push_back(TokenType(u"==", TOKEN::EQ));
    Token.push_back(TokenType(u"!=", TOKEN::NE));
    Token.push_back(TokenType(u"===", TOKEN::EQ_STRICT));
    Token.push_back(TokenType(u"!==", TOKEN::NE_STRICT));
    Token.push_back(TokenType(u">", TOKEN::GT));
    Token.push_back(TokenType(u"<", TOKEN::LT));
    Token.push_back(TokenType(u">=", TOKEN::GTE));
    Token.push_back(TokenType(u"<=", TOKEN::LTE));
    Token.push_back(TokenType(u"&&", TOKEN::AND));
    Token.push_back(TokenType(u"||", TOKEN::OR));
  }
  Node* VarDeclarationStatement::Execute() {
    return new Node();
  };
  Node* AtomExpr::Execute() {
    Node* result = data;
    return result;
  };
  void VarDeclarationStatement::Print() {
    DEBUG << TokenToString(type) << endl;
    for (unsigned long i = 0; i < vars.size(); ++i) {
      DEBUG << vars[i].first << endl;
      vars[i].second -> Print();
    }
  };
  void AtomExpr::Print() {
    DEBUG << TokenToString(type_) << endl;
    data->Print();
  };
  void StringNode::Print() {
    DEBUG << TokenToString(type_) << endl;
    DEBUG << val << endl;
  };
}
