#include <noop_parser.h>
#include <noop_io.h>
#include <noop_type.h>
#include <noop_context.h>
#include <noop_pool.h>
#include <noop.h>

#include <stdexcept>
#include <sstream>
#include <string>


using namespace std;

/* LL(1) parser for simplified JavaScript. */

namespace noop {
Context* current_context = global_context;

/* I/O Helper */

ostream& operator <<(ostream& _out, Token* token) {
  /*
    EndOfSource,
    Keyword,
    Identifier,
    BooleanLiteral,
    NullLiteral,
    NumericLiteral,
    StringLiteral,
    Punctuator
  */
  switch (token->type) {
  case TokenType::EndOfSource:
    _out << "EndOfSource { }";
    break;
  case TokenType::Keyword:
    _out << "Keyword { name: " <<
    ((IdentifierToken *)token)->name << ", start: " << token->start
    << ", end: " << token->end << " }";
    break;
  case TokenType::Identifier:
    _out << "Identifier { name: " <<
    ((IdentifierToken *)token)->name << ", start: " << token->start
    << ", end: " << token->end << " }";
    break;
  case TokenType::Punctuator:
    _out << "Punctuator { value: " <<
    ((PunctuatorToken *)token)->value << ", start: " << token->start
    << ", end: " << token->end << " }";
    break;
  case TokenType::NullLiteral:
    _out << "NullLiteral { start: " << token->start
    << ", end: " << token->end << " }";
    break;
  case TokenType::NumericLiteral:
    _out << "NumericLiteral { value: " <<
    ((NumericLiteralToken *)token)->value << ", start: " << token->start
    << ", end: " << token->end << " }";
    break;
  case TokenType::StringLiteral:
    _out << "StringLiteral { value: " <<
    ((StringLiteralToken *)token)->value << ", start: " << token->start
    << ", end: " << token->end << " }";
  default:
    _out << "~Token { }";
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
    enum {
      NullLiteral,
      BooleanLiteral,
      StringLiteral,
      NumericLiteral,
      Identifier,
      ThisExpression,
      MemberExpression,
      CallExpression,
      AssignmentExpression,
      SequenceExpression,
      BinaryExpression,
      FunctionExpression,
      VariableDeclarator,
      VariableStatement,
      IfStatement,
      WhileStatement,
      ExpressionStatement,
      BlockStatement,
      Body,
      Program
    };
  */
  if (node == nullptr) {
    _out << "NULL";
    return _out;
  }
  bool flag = false;
  switch (node->type) {
  case SyntaxTreeNodeType::NullLiteral:
    _out << "NullLiteralNode { }";
    break;
  case SyntaxTreeNodeType::BooleanLiteral:
    _out << "BooleanLiteralNode { value: " << ((BooleanLiteral*)node)->value <<
    " }";
    break;
  case SyntaxTreeNodeType::StringLiteral:
    _out << "StringLiteralNode { value: " << ((StringLiteral*)node)->value <<
    " }";
    break;
  case SyntaxTreeNodeType::NumericLiteral:
    _out << "NumericLiteralNode { value: " << ((NumericLiteral*)node)->value <<
    " }";
    break;
  case SyntaxTreeNodeType::Identifier:
    _out << "IdentifierNode { name: " << ((Identifier*)node)->name <<
    " }";
    break;
  case SyntaxTreeNodeType::ThisExpression:
    _out << "ThisExpressionNode { }";
    break;
  case SyntaxTreeNodeType::MemberExpression:
    _out << "MemberExpressionNode { left: " << ((MemberExpression*)node)->left <<
    ", _operator: " << ((MemberExpression*)node)->_operator << ", right: " <<
    ((MemberExpression*)node)->right << " }";
    break;
  case SyntaxTreeNodeType::CallExpression:
    _out << "CallExpressionNode { callee: " << ((CallExpression*)node)->callee <<
    ", arguments: " << ((CallExpression*)node)->arguments << " }";
    break;
  case SyntaxTreeNodeType::FunctionExpression:
    _out << "FunctionExpressionNode { name: " << ((FunctionExpression*)node)->id <<
    ", params: " << ((FunctionExpression*)node)->params << ", body: " <<
    ((FunctionExpression*)node)->body << " }";
    break;
  case SyntaxTreeNodeType::ObjectExpression:
    _out << "ObjectExpression { ";
    for (auto& property: ((ObjectExpression*)node)->properties) {
      if (flag) { _out << ", "; }
      _out << property.first << ": " << property.second;
      flag = true;
    }
    _out << " }";
    break;
  case SyntaxTreeNodeType::ArrayExpression:
    _out << "ArrayExpression { elements: " << ((ArrayExpression*)node)->elements <<
    " }";
    break;
  case SyntaxTreeNodeType::AssignmentExpression:
    _out << "AssignmentExpressionNode { left: " << ((AssignmentExpression*)node)->left <<
    ", _operator: " << ((AssignmentExpression*)node)->_operator << ", right: " <<
    ((AssignmentExpression*)node)->right << " }";
    break;
  case SyntaxTreeNodeType::BinaryExpression:
    _out << "BinaryExpressionNode { left: " << ((BinaryExpression*)node)->left <<
    ", _operator: " << ((BinaryExpression*)node)->_operator << ", right: " <<
    ((BinaryExpression*)node)->right << " }";
    break;
  case SyntaxTreeNodeType::VariableDeclarator:
    _out << "VariableDeclaratorNode { id: " <<
    ((VariableDeclarator*)node)->id << ", init: " << ((VariableDeclarator*)node)->init <<
    " }";
    break;
  case SyntaxTreeNodeType::VariableStatement:
    _out << "VariableStatementNode { declarations: " <<
    ((VariableStatement*)node)->declarations << " }";
    break;
  case SyntaxTreeNodeType::IfStatement:
    _out << "IfStatementNode { condition: " << ((IfStatement*)node)->condition <<
    ", consequent: " << ((IfStatement*)node)->consequent << ", alternate: " <<
    ((IfStatement*)node)->alternate << " }";
    break;
  case SyntaxTreeNodeType::WhileStatement:
    _out << "WhileStatementNode { test: " << ((WhileStatement*)node)->test <<
    ", body: " << ((WhileStatement*)node)->body << " }";
    break;
  case SyntaxTreeNodeType::BlockStatement:
    _out << "BlockStatementNode { statements: " <<
    ((BlockStatement*)node)->statements << " }";
    break;
  case SyntaxTreeNodeType::ExpressionStatement:
    _out << "ExpressionStatement { expression: " <<
    ((ExpressionStatement*)node)->expression << " }";
    break;
  case SyntaxTreeNodeType::Body:
    _out << "BodyNode { statements: " <<
    ((Body*)node)->statements << " }";
    break;
  case SyntaxTreeNodeType::Program:
    _out << "ProgramNode { body: " <<
    ((Program*)node)->body << " }";
    break;
  default:
    _out << "~Node { }";
    break;
  }
  return _out;
}

ostream& operator <<(ostream& _out, Object* obj) {
  /*
  enum {
    Object,
    UndefinedObject,
    NullObject,
    StringObject,
    NumericObject,
    BooleanObject,
    NaNObject,
    ErrorObject
  }
  */
  switch (obj->type) {
  case ObjectType::Object:
    _out << "BasicObject { }";
    break;
  case ObjectType::UndefinedObject:
    _out << "UndefinedObject { }";
    break;
  case ObjectType::NullObject:
    _out << "NullObject { }";
    break;
  case ObjectType::FunctionObject:
    _out << "FunctionObject { }";
    break;
  case ObjectType::StringObject:
    _out << "StringObject { value: " << ((StringObject*)obj)->value << " }";
    break;
  case ObjectType::NumericObject:
    _out << "NumericObject { value: " << ((NumericObject*)obj)->value << " }";
    break;
  case ObjectType::BooleanObject:
    _out << "BooleanObject { value: " << (((BooleanObject*)obj)->value ? "true" : "false") << " }";
    break;
  case ObjectType::NaNObject:
    _out << "NaNObject { }";
    break;
  case ObjectType::BlackMagicObject:
    _out << "BlackMagicObject { }";
    break;
  }
  return _out;
}

/* Syntax Tree Resolver */
VariableDeclarator* SyntaxTree::CreateVariableDeclarator(Identifier* id,
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

BlockStatement* SyntaxTree::CreateBlockStatement(vector<Statement*> statements) {
  BlockStatement* node = new BlockStatement();
  node->statements = statements;
  return node;
}

IfStatement* SyntaxTree::CreateIfStatement(Expression* expr,
                                  Statement* consequent, Statement* alternate) {
  IfStatement* node = new IfStatement();
  node->condition = expr;
  node->consequent = consequent;
  node->alternate = alternate;
  return node;
}

WhileStatement* SyntaxTree::CreateWhileStatement(Expression* test,
                                                  Statement* body) {
  WhileStatement* node = new WhileStatement();
  node->test = test;
  node->body = body;
  return node;
}

AssignmentExpression* SyntaxTree::CreateAssignmentExpression(String op,
                                    Expression* left, Expression* right) {
  AssignmentExpression* node = new AssignmentExpression();
  node->_operator = op;
  node->left = left;
  node->right = right;
  return node;
}

ObjectExpression* SyntaxTree::CreateObjectExpression(vector<pair<StringLiteral*, Expression*>> properties) {
  ObjectExpression* node = new ObjectExpression();
  node->properties = properties;
  return node;
}

ArrayExpression* SyntaxTree::CreateArrayExpression(vector<Expression*> elements) {
  ArrayExpression* node = new ArrayExpression();
  node->elements = elements;
  return node;
}

SequenceExpression* SyntaxTree::CreateSequenceExpression(vector<Expression*> expressions) {
  SequenceExpression* node = new SequenceExpression();
  node->expressions = expressions;
  return node;
}

ExpressionStatement* SyntaxTree::CreateExpressionStatement(Expression* expr) {
  ExpressionStatement* node = new ExpressionStatement();
  node->expression = expr;
  return node;
}

MemberExpression* SyntaxTree::CreateMemberExpression(String op, Expression* expr,
                                                     Expression* property) {
  MemberExpression* node = new MemberExpression();
  node->_operator = op;
  node->left = expr;
  node->right = property;
  return node;
}

CallExpression* SyntaxTree::CreateCallExpression(Expression* expr, vector<Expression*> args) {
  CallExpression* node = new CallExpression();
  node->callee = expr;
  node->arguments = args;
  return node;
}

FunctionExpression* SyntaxTree::CreateFunctionExpression(Identifier* id,
                             vector<Expression*> params, Statement* body) {
  FunctionExpression* node = new FunctionExpression();
  node->id = id;
  node->params = params;
  node->body = body;
  return node;
}

BinaryExpression* SyntaxTree::CreateBinaryExpression(String op,
                                          Expression* left, Expression* right) {
  BinaryExpression* node = new BinaryExpression();
  node->_operator = op;
  node->left = left;
  node->right = right;
  return node;
}

Program* SyntaxTree::CreateProgram(Body* body) {
  Program* node = new Program();
  node->body = body;
  return node;
}

Literal* SyntaxTree::CreateLiteral(Token* token) {
  if (token->type == TokenType::NumericLiteral) {
    NumericLiteral* node = new NumericLiteral();
    node->value = ((NumericLiteralToken*)token)->value;
    return node;
  } else if (token->type == TokenType::NullLiteral) {
    NullLiteral* node = new NullLiteral();
    return node;
  } else if (token->type == TokenType::StringLiteral) {
    StringLiteral* node = new StringLiteral();
    node->value = ((StringLiteralToken*)token)->value;
    return node;
  } else if (token->type == TokenType::BooleanLiteral) {
    BooleanLiteral* node = new BooleanLiteral();
    node->value = ((IdentifierToken*)token)->name == U"true";
    return node;
  } else {
    assert(false);
  }
}

Identifier* SyntaxTree::CreateIdentifier(Token* token) {
  Identifier* node = new Identifier();
  node->name = ((IdentifierToken *)token)->name;
  return node;
}

ThisExpression* SyntaxTree::CreateThisExpression() {
  ThisExpression* node = new ThisExpression();
  return node;
}

bool Parser::IsPunctuation(const String& value) {
  return look_ahead->type == TokenType::Punctuator &&
    ((PunctuatorToken *)look_ahead)->value == value;
}

bool Parser::IsKeyword(const String& value) {
  return look_ahead->type == TokenType::Keyword &&
    ((IdentifierToken *)look_ahead)->name == value;
}

bool Parser::IsIdentifierName(Token* token) {
  return token->type == TokenType::Identifier ||
         token->type == TokenType::Keyword ||
         token->type == TokenType::BooleanLiteral ||
         token->type == TokenType::NullLiteral;
}

void Parser::SkipUselessness() {
  Char c, c2;
  bool blocking = false, lining = false;
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
      c2 = source[index + 1];
      if (c2 == '/') {
        index += 2;
        lining = true;
      } else if (c2 == '*') {
        index += 2;
        blocking = true;
      } else {
        break;
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
    throw runtime_error("GetKeyword: Unexpected word " + Encoding::UTF32ToUTF8(keyword) +
                        ".");
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
      case '\\':
        str += U'\\';
        break;
      case '\'':
        str += U'\'';
        break;
      case '"':
        str += U'"';
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
    if (!CharType::IsDigit(c) && c != U'.') {
      --index;
      break;
    }
    str += c;
  }
  NumericLiteralToken* token = new NumericLiteralToken(TokenType::NumericLiteral,
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
  /* 2x: ==, &&, ||, >=, <=, != */
  if (c == c2 && (c == '=' || c == '&' || c == '|')) {
    index += 2;
    PunctuatorToken *token = new PunctuatorToken(TokenType::Punctuator,
                                                 start, index);
    token->value = token->value + c + c2;
    return token;
  }
  if (c2 == '=' && (c == '>' || c == '<' || c == '!')) {
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
  throw runtime_error("GetPunctuator: Unexcepted token.");
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
  Expression* init = nullptr;
  if (IsPunctuation(U"=")) {
    Lex();
    init = ParseAssignmentExpression();
  }
  return delegate.CreateVariableDeclarator(delegate.CreateIdentifier(id), init);
}

vector<VariableDeclarator *> Parser::ParseVariableDeclarationList() {
  vector<VariableDeclarator *> res;
  do {
    if (look_ahead->type != TokenType::Identifier) {
      throw runtime_error("Parse VariableDeclationList: Declaration needs an identifier.");
    }
    res.push_back(ParseVariableDeclarator());
    DEBUG << "A var declarator pushed back, look_head " << look_ahead << endl;
    if (!IsPunctuation(U",")) {
      break;
    }
    Lex();
  } while (index < length);
  DEBUG << "Variable declaration list size: " << res.size() << endl;
  return res;
}

VariableStatement* Parser::ParseVariableStatement() {
  GetKeyword(U"var");
  vector<VariableDeclarator *> declarations;
  declarations = ParseVariableDeclarationList();
  GetSemicolon();
  return delegate.CreateVariableStatement(U"var", declarations);
}

IfStatement* Parser::ParseIfStatement() {
  GetKeyword(U"if");
  assert(IsPunctuation(U"("));
  Lex();
  Expression* cond = ParseExpression();
  assert(IsPunctuation(U")"));
  Lex();
  Statement* consequent = ParseStatement();
  Statement* alternate = nullptr;
  if (IsKeyword(U"else")) {
    Lex();
    alternate = ParseStatement();
  }
  return delegate.CreateIfStatement(cond, consequent, alternate);
}

WhileStatement* Parser::ParseWhileStatement() {
  GetKeyword(U"while");
  assert(IsPunctuation(U"("));
  Lex();
  Expression* test = ParseExpression();
  assert(IsPunctuation(U")"));
  Lex();
  Statement* body = ParseStatement();
  return delegate.CreateWhileStatement(test, body);
}

Expression* Parser::ParseFunctionExpression() {
  GetKeyword(U"function");
  Identifier* id = nullptr;
  if (IsIdentifierName(look_ahead)) {
    id = delegate.CreateIdentifier(Lex());
  }
  assert(IsPunctuation(U"("));
  Lex(); // (
  vector<Expression*> params = ParseArguments();
  assert(IsPunctuation(U")"));
  Lex(); // )
  BlockStatement* body = ParseBlockStatement();
  return delegate.CreateFunctionExpression(id, params, body);
}

Expression* Parser::ParseObjectExpression() {
  Lex(); // {
  vector<pair<StringLiteral*, Expression*>> properties;
  while (!IsPunctuation(U"}")) {
    // key
    Token* token = Lex();
    String value = U"";
    if (token->type == TokenType::StringLiteral) {
      value = ((StringLiteralToken*)token)->value;
    } else if (token->type == TokenType::NumericLiteral) {
      stringstream sio;
      string _value = "";
      sio << ((NumericLiteralToken*)token)->value;
      sio >> _value;
      value = Encoding::UTF8ToUTF32(_value);
    } else if (token->type == TokenType::Identifier) {
      value = ((IdentifierToken*)token)->name;
    } else {
      throw runtime_error("Parse ObjectExpression: Invalid object property name.");
    }
    StringLiteral* key_node = new StringLiteral();
    key_node->value = value;
    // :
    assert(IsPunctuation(U":"));
    Lex();
    // value
    Expression* value_node = ParsePrimaryExpression();
    properties.push_back(make_pair(key_node, value_node));
    if (!IsPunctuation(U"}")) {
      // ,
      assert(IsPunctuation(U","));
      Lex();
    }
  }
  Lex(); // }
  return delegate.CreateObjectExpression(properties);
}

Expression* Parser::ParseArrayExpression() {
  Lex(); // [
  vector<Expression*> elements;
  while (!IsPunctuation(U"]")) {
    // value
    Expression* element = ParsePrimaryExpression();
    elements.push_back(element);
    if (!IsPunctuation(U"]")) {
      // ,
      assert(IsPunctuation(U","));
      Lex();
    }
  }
  Lex(); // ]
  return delegate.CreateArrayExpression(elements);
}

Expression* Parser::ParsePrimaryExpression() {
  int type = look_ahead->type;
  if (type == TokenType::Identifier) {
    return delegate.CreateIdentifier(Lex());
  }
  if (type == TokenType::StringLiteral || type == TokenType::NumericLiteral ||
      type == TokenType::BooleanLiteral || type == TokenType::NullLiteral) {
    return delegate.CreateLiteral(Lex());
  }
  if (type == TokenType::Keyword) {
    if (IsKeyword(U"this")) {
      Lex();
      return delegate.CreateThisExpression();
    } else if (IsKeyword(U"function")) {
      return ParseFunctionExpression();
    }
  }
  if (type == TokenType::Punctuator) {
    if (IsPunctuation(U"{")) {
      return ParseObjectExpression();
    } else if (IsPunctuation(U"[")) {
      return ParseArrayExpression();
    }
  }
  throw runtime_error("Parse PrimaryExpression: Unexcepted token.");
}

Identifier* Parser::ParseLiteralProperty() {
  Token* token = Lex();
  if (!IsIdentifierName(token)) {
    throw runtime_error("Parse LiteralProperty: Unexcepted token.");
  }
  return delegate.CreateIdentifier(token);
}

Expression* Parser::ParseComputedProperty() {
  return ParseExpression();
}

vector<Expression*> Parser::ParseArguments() {
  vector<Expression*> args;
  while (index < length && !IsPunctuation(U")")) {
    args.push_back(ParseAssignmentExpression());
    if (IsPunctuation(U")")) {
      break;
    }
    assert(IsPunctuation(U","));
    Lex();
  }
  return args;
}

Expression* Parser::ParsePostfixExpression() {
  SkipUselessness();
  Expression* expr = ParsePrimaryExpression();
  while (IsPunctuation(U".") || IsPunctuation(U"[") ||
         IsPunctuation(U"(")) {
    /* property */
    if (IsPunctuation(U".")) {
      Lex(); // .
      Expression* property = ParseLiteralProperty();
      expr = delegate.CreateMemberExpression(U".", expr, property);
    } else if (IsPunctuation(U"[")) {
      Lex(); // [
      Expression* property = ParseComputedProperty();
      expr = delegate.CreateMemberExpression(U"[", expr, property);
      Lex(); // ]
    } else if (IsPunctuation(U"(")) {
      Lex(); // (
      vector<Expression*> args = ParseArguments();
      expr = delegate.CreateCallExpression(expr, args);
      Lex(); // )
    }
  }
  return expr;
}

int GetPrecedence(Token* token) {
  int ret = 0;
  if (token->type != TokenType::Punctuator) {
    return 0;
  }
  String value = ((PunctuatorToken*)token)->value;
  if (value == U"||") {
    ret = 1;
  } else if (value == U"&&") {
    ret = 2;
  } else if (value == U"==" || value == U"!=" || value == U"===" || value == U"!==") {
    ret = 3;
  } else if (value == U"<" || value == U">" || value == U"<=" || value == U">=") {
    ret = 4;
  } else if (value == U"<<" || value == U">>") {
    ret = 5;
  } else if (value == U"+" || value == U"-") {
    ret = 6;
  } else if (value == U"*" || value == U"/" || value == U"%") {
    ret = 7;
  }
  return ret;
}

/* https://github.com/jquery/esprima/blob/master/src/parser.ts#L1372 */
Expression* Parser::ParseBinaryExpression() {
  Expression* expr = ParsePostfixExpression();
  Token* token = look_ahead;
  int precedence = GetPrecedence(token);
  if (precedence == 0) {
    return expr;
  }
  Lex();
  Expression* left;
  Expression* right;
  String _operator;
  vector<int> precedence_stack;
  vector<Token*> operator_stack;
  vector<Expression*> expression_stack;
  expression_stack.push_back(expr);
  precedence_stack.push_back(precedence);
  operator_stack.push_back(token);
  expression_stack.push_back(ParsePostfixExpression());
  while ((precedence = GetPrecedence(look_ahead)) > 0) {
    while ((expression_stack.size() > 1) &&
           (precedence <= precedence_stack[precedence_stack.size() - 1])) {
      right = expression_stack[expression_stack.size() - 1];
      expression_stack.pop_back();
      _operator = ((PunctuatorToken *)operator_stack[operator_stack.size() - 1])->value;
      operator_stack.pop_back();
      precedence_stack.pop_back();
      left = expression_stack[expression_stack.size() - 1];
      expression_stack.pop_back();
      expression_stack.push_back(delegate.CreateBinaryExpression(_operator, left, right));
    }
    token = Lex();
    precedence_stack.push_back(precedence);
    operator_stack.push_back(token);
    expression_stack.push_back(ParsePostfixExpression());
  }
  int i = expression_stack.size() - 1;
  expr = expression_stack[i];
  for (; i > 0; --i) {
    expr = delegate.CreateBinaryExpression(((PunctuatorToken*)operator_stack[i - 1])->value,
                                           expression_stack[i - 1], expr);
  }
  return expr;
}

Expression* Parser::ParseAssignmentExpression() {
  Expression* left;
  Expression* right;
  Token* token = look_ahead;
  left = ParseBinaryExpression();
  if (IsPunctuation(U"=")) {
    token = Lex();
    right = ParseAssignmentExpression();
    return delegate.CreateAssignmentExpression(((PunctuatorToken *)token)->value,
                                               left, right);
  }
  return left;
}

Expression* Parser::ParseExpression() {
  Expression* expr =  ParseAssignmentExpression();
  if (IsPunctuation(U",")) {
    vector<Expression*> expressions;
    expressions.push_back(expr);
    while (index < length) {
      if (!IsPunctuation(U",")) {
        break;
      }
      Lex();
      expressions.push_back(ParseAssignmentExpression());
    }
    return delegate.CreateSequenceExpression(expressions);
  }
  return expr;
}

Statement* Parser::ParseStatement() {
  int type = look_ahead->type;
  if (type == TokenType::EndOfSource)
    return nullptr;
  if (type == TokenType::Keyword) {
    String value = ((IdentifierToken *)look_ahead)->name;
    if (value == U"var") {
      return ParseVariableStatement();
    } else if (value == U"if") {
      return ParseIfStatement();
    } else if (value == U"while") {
      return ParseWhileStatement();
    }
  }
  if (type == TokenType::Punctuator) {
    String value = ((PunctuatorToken *)look_ahead)->value;
    // if (value == ";")
    if (value == U"{") {
      return ParseBlockStatement();
    } else if (value == U"}") {
      return nullptr;
    }  else if (value == U"(")
      return delegate.CreateExpressionStatement(ParseExpression());
  }
  Expression* expr = ParseExpression();
  GetSemicolon();
  return delegate.CreateExpressionStatement(expr);
}

/*
 * Parse program block
 */
BlockStatement* Parser::ParseBlockStatement() {
  Statement* statement;
  vector<Statement *> statements;
  assert(IsPunctuation(U"{"));
  Lex();
  while (index < length) {
    if ((statement = ParseStatement()) != nullptr) {
      // DEBUG << "Parsed statement type: " << statement->type << endl;
      statements.push_back(statement);
    } else {
      break;
    }
  }
  assert(IsPunctuation(U"}"));
  Lex();
  return delegate.CreateBlockStatement(statements);
}

/*
 * Parse program body
 */
Body* Parser::ParseBody() {
  Statement* statement;
  Body* body = new Body();
  while (index < length) {
    if ((statement = ParseStatement()) != nullptr) {
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
  look_ahead = nullptr;
  Peek();
  SyntaxTreeNode* node = delegate.CreateProgram(ParseBody());
  DEBUG << "Parsed program: " << endl << node << endl;
  return (Program *)node;
}

SyntaxTree delegate;

bool IsFalse(int pos) {
  if ((MemPool.At(pos)->type == ObjectType::BooleanObject) &&
      (!(((BooleanObject*)MemPool.At(pos))->value))) {
    return true;
  }
  if (MemPool.At(pos)->type == ObjectType::NullObject ||
      MemPool.At(pos)->type == ObjectType::UndefinedObject ||
      MemPool.At(pos)->type == ObjectType::NaNObject) {
    return true;
  }
  if ((MemPool.At(pos)->type == ObjectType::NumericObject) &&
      (!(((NumericObject*)MemPool.At(pos))->value))) {
    return true;
  }
  return false;
}

bool IsInt(Number value) {
  if ((int)(value) == value) {
    return true;
  }
  return false;
}

void PrintVarTable() {
  DEBUG << "========Current Variable Table========" << endl;
  for (auto& entry: current_context->var_table) {
    String str;
    if (MemPool.At(entry.second)->ToString(str)) {
      DEBUG << entry.first << ": " << str << endl;
    } else {
      DEBUG << entry.first << ": " << "[Object object]" << endl;
    }
  }
  DEBUG << "======================================" << endl;
}

int VariableDeclarator::Execute() {
  if (init == nullptr) {
    current_context->var_table[id->name] = 0;
  } else {
    current_context->var_table[id->name] = init->Execute();
    DEBUG << id->name << " is set to " <<
             MemPool.At(current_context->var_table[id->name]) << endl;
  }
  PrintVarTable();
  return 0;
}

int VariableStatement::Execute() {
  for (auto declaration: declarations) {
    declaration->Execute();
  }
  return 0;
}

int ExpressionStatement::Execute() {
  return expression->Execute();
}

int Body::Execute() {
  int ret;
//  current_context = new Context(current_context);
//  DEBUG << "Body Execute: switching context " << current_context->father <<
//    " to " << current_context << endl;
  for (auto& statement: statements) {
    ret = statement->Execute();
  }
//  DEBUG << "Body Execute end: switching context " << current_context <<
//    " to " << current_context->father << endl;
//  current_context = current_context->father;
  return ret;
}

int Program::Execute() {
  DEBUG << "Program Execute: switching context " << current_context <<
    " to [global]" << global_context << endl;
  Context* old_context = current_context;
  current_context = global_context;
  int ret = 0;
  if (body != nullptr) {
    ret = body->Execute();
  }
  current_context = old_context;
  DEBUG << "Program Execute end: switching context [global]" << current_context <<
    " to " << old_context << endl;
  return ret;
}
int Identifier::Execute() {
  return current_context->LookUp(name);
}

int ArrayExpression::Execute() {
  vector<int> _elements;
  for (auto& element: elements) {
    _elements.push_back(element->Execute());
  }
  ArrayObject* obj = new ArrayObject(_elements);
  pool.push_back(obj);
  return pool.size() - 1;
}

int ThisExpression::Execute() {
  return 0;
}

int MemberExpression::Execute() {
  size_t res = 0;
  if(left->type == SyntaxTreeNodeType::Identifier)
    res = current_context->LookUp(((Identifier*)left)->name);
  else if (left->type == SyntaxTreeNodeType::MemberExpression) {
    res = left->Execute();
  }
  if (res == 0)
    throw runtime_error("Can not find property " + Encoding::UTF32ToUTF8(((Identifier*)right)->name) + " of undefined");
  return MemPool.At(res)->JumpToProperty(((Identifier*)right)->name);
}

int CallExpression::Execute() {
  int callee_pos = callee->Execute(), ret = 0;
  Context* old_context = current_context;
  current_context = new Context(current_context);
  DEBUG << "Function call: switching context " << old_context <<
    " to " << current_context << endl;
  /* Special NativeFunctionObject */
  if (MemPool.At(callee_pos)->type == ObjectType::NativeFunctionObject) {
    DEBUG << "Call native function: " << ((NativeFunctionObject*)MemPool.At(callee_pos))->name << endl;
    vector<Object*> args;
    for (auto& arg: arguments) {
      args.push_back(MemPool.At(arg->Execute()));
    }
    ret = (*(((NativeFunctionObject*)MemPool.At(callee_pos))->function))(args);
  } else if (MemPool.At(callee_pos)->type == ObjectType::FunctionObject) {
    std::vector<String> params = ((FunctionObject*)MemPool.At(callee_pos))->params;
    for (size_t i = 0; i < params.size(); ++i) {
      if (i >= arguments.size()) {
        Object *res = new UndefinedObject();
        pool.push_back(res);
        current_context->var_table[params[i]] = pool.size() - 1;
      } else {
        current_context->var_table[params[i]] = arguments[i]->Execute();
      }
    }
    ret = ((FunctionObject*)MemPool.At(callee_pos))->function->Execute();
  } else {
    throw runtime_error("Callee is not a function");
  }
  DEBUG << "Function call end: switching context " << current_context <<
    " to " << old_context << endl;
  current_context = old_context;
  return ret;
}

int AssignmentExpression::Execute() {
  if (left->type == SyntaxTreeNodeType::Identifier) {
    size_t id = current_context->LookUp(((Identifier*)left)->name);
    DEBUG << "Find " << ((Identifier*)left)->name << ": " << id << endl;
    if (id > 0) {
      // found
      MemPool.At(id) = MemPool.At(right->Execute());
      DEBUG << ((Identifier*)left)->name << " is set to " << MemPool(id) << endl;
    } else {
      // not found
      throw runtime_error(Encoding::UTF32ToUTF8(((Identifier*)left)->name) +
        " is not defined");
    }
  }
  else if (left->type == SyntaxTreeNodeType::MemberExpression) {
    int id = left->Execute(), right_id = right->Execute();
    MemPool.At(id) = MemPool.At(right_id);
    DEBUG << "Left MemberExpression is set to " << MemPool.At(id) << endl;
  }
  PrintVarTable();
  return 0;
}

int SequenceExpression::Execute() {
  for (auto& expression: expressions) {
    expression->Execute();
  }
  return pool.size() - 1;
}

int BinaryExpression::Execute() {
  int left_pos = left->Execute();
  int right_pos = right->Execute();
  if (_operator == U"+") {
    if (MemPool.At(left_pos)->type == ObjectType::StringObject ||
        MemPool.At(right_pos)->type == ObjectType::StringObject) {
      String left_value, right_value;
      if ((!MemPool.At(left_pos)->ToString(left_value)) ||
          (!MemPool.At(right_pos)->ToString(right_value))) {
        Object *res = new NaNObject();
        pool.push_back(res);
        return pool.size() - 1;
      };
      Object *res = new StringObject(
        left_value + right_value
      );
      pool.push_back(res);
      return pool.size() - 1;
    } else if (MemPool.At(left_pos)->type == ObjectType::NumericObject ||
               MemPool.At(right_pos)->type == ObjectType::NumericObject) {
      Number left_value, right_value;
      if ((!MemPool.At(left_pos)->ToNumber(left_value)) ||
          (!MemPool.At(right_pos)->ToNumber(right_value))) {
        Object *res = new NaNObject();
        pool.push_back(res);
        return pool.size() - 1;
      };
      Object *res = new NumericObject(
        left_value + right_value
      );
      pool.push_back(res);
      return pool.size() - 1;
    }
  } else if (_operator == U"-") {
    Number left_value, right_value;
    if ((!MemPool.At(left_pos)->ToNumber(left_value)) ||
        (!MemPool.At(right_pos)->ToNumber(right_value))) {
      Object *res = new NaNObject();
      pool.push_back(res);
      return pool.size() - 1;
    };
    Object *res = new NumericObject(
      left_value - right_value
    );
    pool.push_back(res);
    return pool.size() - 1;
  } else if (_operator == U"*") {
    Number left_value, right_value;
    if ((!MemPool.At(left_pos)->ToNumber(left_value)) ||
        (!MemPool.At(right_pos)->ToNumber(right_value))) {
      Object *res = new NaNObject();
      pool.push_back(res);
      return pool.size() - 1;
    };
    Object *res = new NumericObject(
      left_value * right_value
    );
    pool.push_back(res);
    return pool.size() - 1;
  } else if (_operator == U"/") {
    Number left_value, right_value;
    if ((!MemPool.At(left_pos)->ToNumber(left_value)) ||
        (!MemPool.At(right_pos)->ToNumber(right_value))) {
      Object *res = new NaNObject();
      pool.push_back(res);
      return pool.size() - 1;
    };
    Object *res = new NumericObject(
      left_value / right_value
    );
    pool.push_back(res);
    return pool.size() - 1;
  } else if (_operator == U"%") {
    Number left_value, right_value;
    if ((!MemPool.At(left_pos)->ToNumber(left_value)) ||
        (!MemPool.At(right_pos)->ToNumber(right_value))) {
      Object *res = new NaNObject();
      pool.push_back(res);
      return pool.size() - 1;
    };
    if (IsInt(left_value) && IsInt(left_value)) {
      Object *res = new NumericObject(
        (int)(left_value) % (int)(right_value)
      );
      pool.push_back(res);
      return pool.size() - 1;
    } else {
      return left_pos;
    }
  } else if (_operator == U"<<") {
    Number left_value, right_value;
    if ((!MemPool.At(left_pos)->ToNumber(left_value)) ||
        (!MemPool.At(right_pos)->ToNumber(right_value))) {
      Object *res = new NaNObject();
      pool.push_back(res);
      return pool.size() - 1;
    };
    if (IsInt(left_value) && IsInt(left_value)) {
      Object *res = new NumericObject(
        (int)(left_value) << (int)(right_value)
      );
      pool.push_back(res);
      return pool.size() - 1;
    } else {
      return left_pos;
    }
  } else if (_operator == U">>") {
    Number left_value, right_value;
    if ((!MemPool.At(left_pos)->ToNumber(left_value)) ||
        (!MemPool.At(right_pos)->ToNumber(right_value))) {
      Object *res = new NaNObject();
      pool.push_back(res);
      return pool.size() - 1;
    };
    if (IsInt(left_value) && IsInt(left_value)) {
      Object *res = new NumericObject(
        (int)(left_value) >> (int)(right_value)
      );
      pool.push_back(res);
      return pool.size() - 1;
    } else {
      return left_pos;
    }
  } else if (_operator == U"==") {
    if (left_pos == right_pos) {
      Object *res = new BooleanObject(true);
      pool.push_back(res);
      return pool.size() - 1;
    } else if (MemPool.At(left_pos)->type == ObjectType::StringObject ||
        MemPool.At(right_pos)->type == ObjectType::StringObject) {
      String left_value, right_value;
      if ((!MemPool.At(left_pos)->ToString(left_value)) ||
          (!MemPool.At(right_pos)->ToString(right_value))) {
        Object *res = new NaNObject();
        pool.push_back(res);
        return pool.size() - 1;
      };
      Object *res = new BooleanObject(
        left_value == right_value
      );
      pool.push_back(res);
      return pool.size() - 1;
    } else if (pool[left_pos]->type == ObjectType::NumericObject ||
               pool[right_pos]->type == ObjectType::NumericObject) {
      Number left_value, right_value;
      if ((!pool[left_pos]->ToNumber(left_value)) ||
          (!pool[right_pos]->ToNumber(right_value))) {
        Object *res = new NaNObject();
        pool.push_back(res);
        return pool.size() - 1;
      };
      Object *res = new BooleanObject(
        left_value == right_value
      );
      pool.push_back(res);
      return pool.size() - 1;
    } else {
      Object *res = new BooleanObject(
        false
      );
      pool.push_back(res);
      return pool.size() - 1;
    }
  } else if (_operator == U"!=") {
    if (left_pos == right_pos) {
      Object *res = new BooleanObject(false);
      pool.push_back(res);
      return pool.size() - 1;
    } else if (pool[left_pos]->type == ObjectType::StringObject ||
        pool[right_pos]->type == ObjectType::StringObject) {
      String left_value, right_value;
      if ((!pool[left_pos]->ToString(left_value)) ||
          (!pool[right_pos]->ToString(right_value))) {
        Object *res = new NaNObject();
        pool.push_back(res);
        return pool.size() - 1;
      };
      Object *res = new BooleanObject(
        left_value != right_value
      );
      pool.push_back(res);
      return pool.size() - 1;
    } else if (pool[left_pos]->type == ObjectType::NumericObject ||
               pool[right_pos]->type == ObjectType::NumericObject) {
      Number left_value, right_value;
      if ((!pool[left_pos]->ToNumber(left_value)) ||
          (!pool[right_pos]->ToNumber(right_value))) {
        Object *res = new NaNObject();
        pool.push_back(res);
        return pool.size() - 1;
      };
      Object *res = new BooleanObject(
        left_value != right_value
      );
      pool.push_back(res);
      return pool.size() - 1;
    } else {
      Object *res = new BooleanObject(
        true
      );
      pool.push_back(res);
      return pool.size() - 1;
    }
  } else if (_operator == U"===") {
    if (pool[left_pos]->type == pool[right_pos]->type) {
      if (pool[left_pos]->type == ObjectType::NumericObject ||
          pool[left_pos]->type == ObjectType::StringObject ||
          pool[left_pos]->type == ObjectType::BooleanObject) {
        String left_value, right_value;
        pool[left_pos]->ToString(left_value);
        pool[right_pos]->ToString(right_value);
        Object *res = new BooleanObject(
          left_value == right_value
        );
        pool.push_back(res);
        return pool.size() - 1;
      } else {
        Object *res = new BooleanObject(
          left_pos == right_pos
        );
        pool.push_back(res);
        return pool.size() - 1;
      }
    } else {
      Object *res = new BooleanObject(
        false
      );
      pool.push_back(res);
      return pool.size() - 1;
    }
  } else if (_operator == U"!==") {
    if (pool[left_pos]->type == pool[right_pos]->type) {
      if (pool[left_pos]->type == ObjectType::NumericObject ||
          pool[left_pos]->type == ObjectType::StringObject ||
          pool[left_pos]->type == ObjectType::BooleanObject) {
        String left_value, right_value;
        pool[left_pos]->ToString(left_value);
        pool[right_pos]->ToString(right_value);
        Object *res = new BooleanObject(
          left_value != right_value
        );
        pool.push_back(res);
        return pool.size() - 1;
      } else {
        Object *res = new BooleanObject(
          left_pos != right_pos
        );
        pool.push_back(res);
        return pool.size() - 1;
      }
    } else {
      Object *res = new BooleanObject(
        true
      );
      pool.push_back(res);
      return pool.size() - 1;
    }
  } else if (_operator == U"<") {
    if (pool[left_pos]->type == ObjectType::StringObject ||
        pool[right_pos]->type == ObjectType::StringObject) {
      String left_value, right_value;
      if ((!pool[left_pos]->ToString(left_value)) ||
          (!pool[right_pos]->ToString(right_value))) {
        Object *res = new NaNObject();
        pool.push_back(res);
        return pool.size() - 1;
      };
      Object *res = new BooleanObject(
        left_value < right_value
      );
      pool.push_back(res);
      return pool.size() - 1;
    } else if (pool[left_pos]->type == ObjectType::NumericObject ||
               pool[right_pos]->type == ObjectType::NumericObject) {
      Number left_value, right_value;
      if ((!pool[left_pos]->ToNumber(left_value)) ||
          (!pool[right_pos]->ToNumber(right_value))) {
        Object *res = new NaNObject();
        pool.push_back(res);
        return pool.size() - 1;
      };
      Object *res = new BooleanObject(
        left_value < right_value
      );
      pool.push_back(res);
      return pool.size() - 1;
    }
  } else if (_operator == U"<=") {
    if (pool[left_pos]->type == ObjectType::StringObject ||
        pool[right_pos]->type == ObjectType::StringObject) {
      String left_value, right_value;
      if ((!pool[left_pos]->ToString(left_value)) ||
          (!pool[right_pos]->ToString(right_value))) {
        Object *res = new NaNObject();
        pool.push_back(res);
        return pool.size() - 1;
      };
      Object *res = new BooleanObject(
        left_value <= right_value
      );
      pool.push_back(res);
      return pool.size() - 1;
    } else if (pool[left_pos]->type == ObjectType::NumericObject ||
               pool[right_pos]->type == ObjectType::NumericObject) {
      Number left_value, right_value;
      if ((!pool[left_pos]->ToNumber(left_value)) ||
          (!pool[right_pos]->ToNumber(right_value))) {
        Object *res = new NaNObject();
        pool.push_back(res);
        return pool.size() - 1;
      };
      Object *res = new BooleanObject(
        left_value <= right_value
      );
      pool.push_back(res);
      return pool.size() - 1;
    }
  } else if (_operator == U">=") {
    if (pool[left_pos]->type == ObjectType::StringObject ||
        pool[right_pos]->type == ObjectType::StringObject) {
      String left_value, right_value;
      if ((!pool[left_pos]->ToString(left_value)) ||
          (!pool[right_pos]->ToString(right_value))) {
        Object *res = new NaNObject();
        pool.push_back(res);
        return pool.size() - 1;
      };
      Object *res = new BooleanObject(
        left_value >= right_value
      );
      pool.push_back(res);
      return pool.size() - 1;
    } else if (pool[left_pos]->type == ObjectType::NumericObject ||
               pool[right_pos]->type == ObjectType::NumericObject) {
      Number left_value, right_value;
      if ((!pool[left_pos]->ToNumber(left_value)) ||
          (!pool[right_pos]->ToNumber(right_value))) {
        Object *res = new NaNObject();
        pool.push_back(res);
        return pool.size() - 1;
      };
      Object *res = new BooleanObject(
        left_value >= right_value
      );
      pool.push_back(res);
      return pool.size() - 1;
    }
  } else if (_operator == U">") {
    if (pool[left_pos]->type == ObjectType::StringObject ||
        pool[right_pos]->type == ObjectType::StringObject) {
      String left_value, right_value;
      if ((!pool[left_pos]->ToString(left_value)) ||
          (!pool[right_pos]->ToString(right_value))) {
        Object *res = new NaNObject();
        pool.push_back(res);
        return pool.size() - 1;
      };
      Object *res = new BooleanObject(
        left_value > right_value
      );
      pool.push_back(res);
      return pool.size() - 1;
    } else if (pool[left_pos]->type == ObjectType::NumericObject ||
               pool[right_pos]->type == ObjectType::NumericObject) {
      Number left_value, right_value;
      if ((!pool[left_pos]->ToNumber(left_value)) ||
          (!pool[right_pos]->ToNumber(right_value))) {
        Object *res = new NaNObject();
        pool.push_back(res);
        return pool.size() - 1;
      };
      Object *res = new BooleanObject(
        left_value > right_value
      );
      pool.push_back(res);
      return pool.size() - 1;
    }
  } else if (_operator == U"&&") {
    if (IsFalse(left_pos)) {
      return left_pos;
    } else {
      return right_pos;
    }
  } else if (_operator == U"||") {
    if (!IsFalse(left_pos)) {
      return left_pos;
    } else {
      return right_pos;
    }
  }
  Object *res = new NaNObject();
  pool.push_back(res);
  return pool.size() - 1;
}

int BlockStatement::Execute() {
  int ret = 0;
  for (auto& statement: statements) {
    ret = statement->Execute();
  }
  return ret;
}

int WhileStatement::Execute() {
  int ret = 0;
  while (!IsFalse(test->Execute())) {
    body->Execute();
    ret += 1;
  }
  NumericObject* num = new NumericObject(ret);
  pool.push_back(num);
  return pool.size() - 1;
}

int NumericLiteral::Execute() {
  Object *res = new NumericObject(value);
  pool.push_back(res);
  return pool.size() - 1;
}

int StringLiteral::Execute() {
  Object *res = new StringObject(value);
  pool.push_back(res);
  return pool.size() - 1;
}

int BooleanLiteral::Execute() {
  Object *res = new BooleanObject(value);
  pool.push_back(res);
  return pool.size() - 1;
}


int NullLiteral::Execute() {
  Object *res = new NullObject();
  pool.push_back(res);
  return pool.size() - 1;
}

int IfStatement::Execute() {
  if (!IsFalse(condition->Execute())) {
    return consequent->Execute();
  } else if (alternate) {
    return alternate->Execute();
  } else {
    return 0;
  }
}

int FunctionExpression::Execute() {
  String name = U"";
  vector<String> _params;
  for (auto& tmp: params) {
    _params.push_back(((Identifier*)tmp)->name);
  }
  if (id != nullptr) {
    name = ((Identifier*)id)->name;
  }
  Object *res = new FunctionObject(name, (BlockStatement*)body, _params);
  pool.push_back(res);
  if (id != NULL) {
    current_context->var_table[id->name] = pool.size() - 1;
    DEBUG << id->name << " is set to " <<
             pool[current_context->LookUp(id->name)] << endl;
  }
  return pool.size() - 1;
}

int ObjectExpression::Execute() {
  std::unordered_map<String, size_t> _properties;
  for (auto& tmp: properties) {
    _properties[tmp.first->value] =
      tmp.second->Execute();
  }
  Object *res = new Object(ObjectType::Object);
  res->properties = _properties;
  pool.push_back(res);
  return pool.size() - 1;
}
/*
struct ObjectExpression: Expression {
  std::vector<std::pair<StringLiteral*, Expression*>> properties;
  ObjectExpression() {
    type = SyntaxTreeNodeType::ObjectExpression;
  }
  int Execute() override {
    return 0;
  }
};
*/
} // namespace noop
