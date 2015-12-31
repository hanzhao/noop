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
    Token.push_back(TokenType(U"EOS", TOKEN::EOS));
    Token.push_back(TokenType(U"(", TOKEN::LPAREN));
    Token.push_back(TokenType(U")", TOKEN::RPAREN));
    Token.push_back(TokenType(U"[", TOKEN::LBRACK));
    Token.push_back(TokenType(U"]", TOKEN::RBRACK));
    Token.push_back(TokenType(U"{", TOKEN::LBRACE));
    Token.push_back(TokenType(U"}", TOKEN::RBRACE));
    Token.push_back(TokenType(U".", TOKEN::PERIOD));
    Token.push_back(TokenType(U",", TOKEN::COMMA));
    Token.push_back(TokenType(U";", TOKEN::SEMICOLON));
    Token.push_back(TokenType(U"var", TOKEN::VAR_DECLARATION));
    Token.push_back(TokenType(U"if", TOKEN::IF));
    Token.push_back(TokenType(U"else", TOKEN::ELSE));
    Token.push_back(TokenType(U"while", TOKEN::WHILE));
    Token.push_back(TokenType(U"expression", TOKEN::EXPRESSION));
    Token.push_back(TokenType(U"function_call", TOKEN::FUNCTION_CALL));
    Token.push_back(TokenType(U"function_declaration", TOKEN::FUNCTION_DECLARATION));
    Token.push_back(TokenType(U"+", TOKEN::ADD));
    Token.push_back(TokenType(U"-", TOKEN::SUB));
    Token.push_back(TokenType(U"*", TOKEN::MUL));
    Token.push_back(TokenType(U"/", TOKEN::DIV));
    Token.push_back(TokenType(U"=", TOKEN::ASSIGN));
    Token.push_back(TokenType(U"==", TOKEN::EQ));
    Token.push_back(TokenType(U"!=", TOKEN::NE));
    Token.push_back(TokenType(U"===", TOKEN::EQ_STRICT));
    Token.push_back(TokenType(U"!==", TOKEN::NE_STRICT));
    Token.push_back(TokenType(U">", TOKEN::GT));
    Token.push_back(TokenType(U"<", TOKEN::LT));
    Token.push_back(TokenType(U">=", TOKEN::GTE));
    Token.push_back(TokenType(U"<=", TOKEN::LTE));
    Token.push_back(TokenType(U"&&", TOKEN::AND));
    Token.push_back(TokenType(U"||", TOKEN::OR));
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
