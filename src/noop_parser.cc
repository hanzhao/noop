#include <noop_parser.h>

#include <stdexcept>
#include <sstream>

#include <noop_io.h>
#include <noop_type.h>

using namespace std;

/* LL(1) parser for simplified JavaScript. */

namespace noop {

/* I/O Helper */

ostream& operator <<(ostream& _out, Token* token) {
  /*
    EndOfSource,
    Keyword,
    Identifier,
    BooleanLiteral,
    NullLiteral,
    NumberLiteral,
    StringLiteral,
    Punctuator
  */
  switch (token->type) {
  case TokenType::EndOfSource:
    _out << "EndOfSource { type: _ }";
    break;
  case TokenType::Keyword:
    _out << "Keyword { type: _, name: " <<
    ((IdentifierToken *)token)->name << ", start: " << token->start
    << ", end: " << token->end << " }";
    break;
  case TokenType::Identifier:
    _out << "Identifier { type: _, name: " <<
    ((IdentifierToken *)token)->name << ", start: " << token->start
    << ", end: " << token->end << " }";
    break;
  case TokenType::Punctuator:
    _out << "Punctuator { type: _, name: " <<
    ((PunctuatorToken *)token)->value << ", start: " << token->start
    << ", end: " << token->end << " }";
    break;
  }
  return _out;
}

template<class T>
ostream& operator <<(ostream& _out, vector<T> v) {
  _out << "[ ";
  for (size_t i = 0; i < v.size(); ++i) {
    _out << v[i];
    if (i != v.size() - 1)
      _out << ", ";
  }
  _out << " ]";
  return _out;
}

ostream& operator <<(ostream& _out, SyntaxTreeNode* node) {
  /*
    VariableDeclaratorNode,
    VariableStatementNode,
    BodyNode,
    ProgramNode
  */
  if (node == NULL) {
    _out << "NULL";
    return _out;
  }
  switch (node->type) {
  case SyntaxTreeNodeType::VariableDeclaratorNode:
    _out << "VariableDeclaratorNode { type: _, id: " <<
    ((VariableDeclarator*)node)->id << ", init: " << ((VariableDeclarator*)node)->init <<
    " }";
    break;
  case SyntaxTreeNodeType::VariableStatementNode:
    _out << "VariableStatementNode { type: _, declarations: " <<
    ((VariableStatement*)node)->declarations << " }";
    break;
  case SyntaxTreeNodeType::BodyNode:
    _out << "BodyNode { type: _, statements: " <<
    ((Body*)node)->statements << " }";
    break;
  case SyntaxTreeNodeType::ProgramNode:
    _out << "ProgramNode { type: _, body: " <<
    ((Program*)node)->body << " }";
    break;
  }
  return _out;
}

/* Syntax Tree Resolver */
VariableDeclarator* SyntaxTree::CreateVariableDeclarator(IdentifierToken* id,
                                                         Expression* init) {
  VariableDeclarator* node = new VariableDeclarator();
  node->id = id;
  node->init = init;
  return node;
}

VariableStatement* SyntaxTree::CreateVariableStatement(String kind,
                                    vector<VariableDeclarator *> declarations) {
  VariableStatement* node = new VariableStatement();
  node->kind = kind;
  node->declarations = declarations;
  return node;
}

ExpressionStatement* SyntaxTree::CreateExpressionStatement(Expression* expr) {
  /* TODO */
}

Program* SyntaxTree::CreateProgram(Body* body) {
  Program* node = new Program();
  node->body = body;
  return node;
}

bool Parser::IsSamePunctuation(const String& value) {
  return look_ahead->type == TokenType::Punctuator &&
    ((PunctuatorToken *)look_ahead)->value == value;
}

void Parser::SkipUselessness() {
  Char c;
  bool blocking, lining;
  while (index < length) {
    c = source[index];
    if (lining) {
      index += 1;
      if (c == '\n') {
        lining = false;
      }
    } else if (blocking) {
      index += 1;
      if (c == '*' && source[index + 1] == '/') {
        index += 1;
        blocking = false;
      }
    } else if (c == '/') {
      c = source[index++];
      if (c == '/') {
        lining = true;
      } else if (c == '*') {
        blocking = true;
      }
    } else if (CharType::IsSpace(c)) {
      index += 1;
    } else {
      break;
    }
  }
}

void Parser::GetKeyword(const String& keyword) {
  Token* token = Lex();
  if (token->type != TokenType::Keyword ||
      ((IdentifierToken *)token)->name != keyword) {
    throw runtime_error("Expected word " + Encoding::UTF32ToUTF8(keyword) +
                        " is not matched.");
  }
}

void Parser::GetSemicolon() {
  if (source[index] == 59) {
    Lex();
    return;
  }
}

/*
 * Token Getter
 */
StringLiteralToken* Parser::GetStringLiteral() {
  int quote = source[index];
  int start = index++;
  String str;
  while (index < length) {
    int c = source[index++];
    /* End of literal */
    if (c == quote) {
      quote = 0;
      break;
    }
    /* \ */
    else if (c == '\\') {
      c = source[index++];
      switch (c) {
      case 'b':
        str += U'\b';
        break;
      case 'f':
        str += U'\f';
        break;
      case 'n':
        str += U'\n';
        break;
      case 't':
        str += U'\t';
        break;
      case 'v':
        str += U'\v';
        break;
      case 'r':
        str += U'\r';
        break;
      default:
        assert(false);
      }
    }
    /* normal */
    else {
      str += c;
    }
  }
  assert(quote == 0);
  StringLiteralToken* token = new StringLiteralToken(TokenType::StringLiteral,
                                                     start, index);
  token->value = str;
  return token;
}

NumericLiteralToken* Parser::GetNumericLiteral() {
  String str = U"";
  int start = index, c;
  while (index < length) {
    c = source[index++];
    if (!CharType::IsDigit(c))
      break;
    str += c;
  }
  NumericLiteralToken* token = new NumericLiteralToken(TokenType::NumberLiteral,
                                                       start, index);
  stringstream(Encoding::UTF32ToUTF8(str)) >> token->value;
  return token;
}

IdentifierToken* Parser::GetIdentifier() {
  String str = U"";
  int start = index, type, c;
  while (index < length) {
    c = source[index];
    if (!CharType::IsIdentifier(c))
      break;
    index += 1;
    str += c;
  }
  if (StringType::IsKeyword(str)) {
    type = TokenType::Keyword;
  } else if (str == U"null") {
    type = TokenType::NullLiteral;
  } else if (str == U"true" || str == U"false") {
    type = TokenType::BooleanLiteral;
  } else {
    type = TokenType::Identifier;
  }
  IdentifierToken* token = new IdentifierToken(type, start, index);
  token->name = str;
  return token;
}

PunctuatorToken* Parser::GetPunctuator() {
  /* 1x-terminal */
  int start = index;
  Char c = source[index];
  if (c == '.' || c == '(' || c == ')' || c == ';' || c == ',' || c == ':' ||
      c == '[' || c == ']' || c == '{' || c == '}' || c == '?' || c == '~') {
    index += 1;
    PunctuatorToken *token = new PunctuatorToken(TokenType::Punctuator,
                                                 start, index);
    token->value = token->value + c;
    return token;
  }
  /* 3x: ===, !== */
  Char c2 = source[index + 1];
  Char c3 = source[index + 2];
  if (c2 == '=' && c3 == '=' && (c == '!' || c == '=')) {
    index += 3;
    PunctuatorToken *token = new PunctuatorToken(TokenType::Punctuator,
                                                 start, index);
    token->value = token->value + c + c2 + c3;
    return token;
  }
  /* 2x: ==, &&, ||, >=, <= */
  if (c == c2 && (c == '=' || c == '&' || c == '|')) {
    index += 2;
    PunctuatorToken *token = new PunctuatorToken(TokenType::Punctuator,
                                                 start, index);
    token->value = token->value + c + c2;
    return token;
  }
  if (c2 == '=' && (c == '>' || c == '<')) {
    index += 2;
    PunctuatorToken *token = new PunctuatorToken(TokenType::Punctuator,
                                                 start, index);
    token->value = token->value + c + c2;
    return token;
  }
  /* 1x */
  if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
      c == '>' || c == '<' || c == '=') {
    index += 1;
    PunctuatorToken *token = new PunctuatorToken(TokenType::Punctuator,
                                                 start, index);
    token->value = token->value + c;
    return token;
  }

}

/*
 * Look Ahead for LL(1)
 */
Token* Parser::LookAhead() {
  SkipUselessness();
  /* This is end of source code */
  if (index >= length) {
    return new Token(TokenType::EndOfSource, index, index);
  }
  Char c = source[index];
  /* String literal */
  if (c == '\'' || c == '"') {
    return GetStringLiteral();
  }
  if (CharType::IsDigit(c)) {
    return GetNumericLiteral();
  }
  if (CharType::IsIdentifier(c)) {
    return GetIdentifier();
  }
  return GetPunctuator();
}
/*
 * Peek next token for LookAhead
 */
void Parser::Peek() {
  int current_index = index;
  look_ahead = LookAhead();
  index = current_index;
}
/*
 * Get this token
 */
Token* Parser::Lex() {
  Token* token = look_ahead;
  index = token->end;
  DEBUG << "Token before LookAhead" << look_ahead << endl;
  DEBUG << "Index before LookAhead: " << token->end << endl;
  look_ahead = LookAhead();
  index = token->end;
  DEBUG << "Token after LookAhead" << look_ahead << endl;
  DEBUG << "Index after LookAhead: " << token->end << endl;
  return token;
}

VariableDeclarator* Parser::ParseVariableDeclarator() {
  IdentifierToken* id = (IdentifierToken*)Lex();
  DEBUG << "Parsed var declarator: " << id << ", look ahead" << look_ahead << endl;
  Expression* init = NULL;
//  if (IsSamePunctuation("=")) {
//    lex();
//    init = ParseAssignmentExpression();
//  }
  return delegate.CreateVariableDeclarator(id, init);
}

vector<VariableDeclarator *> Parser::ParseVariableDeclarationList(const String& kind) {
  vector<VariableDeclarator *> res;
  do {
    if (look_ahead->type != TokenType::Identifier) {
      throw runtime_error("Var declaration needs an identifier");
    }
    res.push_back(ParseVariableDeclarator());
    DEBUG << "A var declarator pushed back, look_head " << look_ahead << endl;
    if (!IsSamePunctuation(U",")) {
      break;
    }
    Lex();
  } while (index < length);
  DEBUG << "Variable declaration list size: " << res.size() << endl;
  return res;
}

VariableStatement* Parser::ParseVariableStatement() {
  vector<VariableDeclarator *> declarations;
  GetKeyword(U"var");
  declarations = ParseVariableDeclarationList(U"var");
  GetSemicolon();
  return delegate.CreateVariableStatement(U"var", declarations);
}

Statement* Parser::ParseStatement() {
  int type = look_ahead->type;
  if (type == TokenType::EndOfSource)
    return NULL;
  if (type == TokenType::Keyword) {
    String value = ((IdentifierToken *)look_ahead)->name;
    if (value == U"var") {
      return ParseVariableStatement();
    }
  }
  if (type == TokenType::Punctuator) {
    String value = ((PunctuatorToken *)look_ahead)->value;
    // if (value == ";")
    if (value == U"{") {
      return ParseBody();
    } else if (value == U"}") {
      return NULL;
    } // else if (value == U"(")
  //    return ParseExpression();
  }
  // expr = ParseExpression();
  // GetSemicolon();
  // return delegate.CreateExpressionStatement(expr);
}

/*
 * Parse program body or block body
 */
Body* Parser::ParseBody() {
  Statement* statement;
  Body* body = new Body();
  // block ?
  if (look_ahead->type == TokenType::Punctuator &&
      ((PunctuatorToken *)look_ahead)->value == U"{") {
    Lex();
  }
  while (index < length) {
    if ((statement = ParseStatement()) != NULL) {
      DEBUG << "Parsed statement type: " << statement->type << endl;
      body->statements.push_back(statement);
    } else {
      break;
    }
  }
  return body;
}

/*
 * Parse an entire JavaScript source code
 */
Program* Parser::ParseProgram(String code) {
  source = code;
  index = 0;
  length = code.length();
  look_ahead = NULL;
  DEBUG << source << endl;
  Peek();
  SyntaxTreeNode* node = delegate.CreateProgram(ParseBody());
  DEBUG << "Parsed program: " << endl << node << endl;
  return (Program *)node;
}

SyntaxTree delegate;

} // namespace noop
