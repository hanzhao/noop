#include <noop_parser.h>

#include <stdexcept>
#include <sstream>

#include <noop_io.h>
#include <noop_type.h>
#include <noop_context.h>

using namespace std;

/* LL(1) parser for simplified JavaScript. */

namespace noop {
Context* current_context;

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
      NumberLiteral,
      Identifier,
      ThisExpression,
      MemberExpression,
      CallExpression,
      AssignmentExpression,
      SequenceExpression,
      BinaryExpression,
      VariableDeclarator,
      VariableStatement,
      ExpressionStatement,
      Body,
      Program
    };
  */
  if (node == NULL) {
    _out << "NULL";
    return _out;
  }
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
  case SyntaxTreeNodeType::NumberLiteral:
    _out << "NumberLiteralNode { value: " << ((NumberLiteral*)node)->value <<
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
    ", arguments: " << ((CallExpression*)node)->arguments << "} ";
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

AssignmentExpression* SyntaxTree::CreateAssignmentExpression(String op,
                                    Expression* left, Expression* right) {
  AssignmentExpression* node = new AssignmentExpression();
  node->_operator = op;
  node->left = left;
  node->right = right;
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
  if (token->type == TokenType::NumberLiteral) {
    NumberLiteral* node = new NumberLiteral();
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
  throw runtime_error("Unknown token");
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
  if (IsPunctuation(U"=")) {
    Lex();
    init = ParseAssignmentExpression();
  }
  return delegate.CreateVariableDeclarator(id, init);
}

vector<VariableDeclarator *> Parser::ParseVariableDeclarationList() {
  vector<VariableDeclarator *> res;
  do {
    if (look_ahead->type != TokenType::Identifier) {
      throw runtime_error("Var declaration needs an identifier");
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
  vector<VariableDeclarator *> declarations;
  GetKeyword(U"var");
  declarations = ParseVariableDeclarationList();
  GetSemicolon();
  return delegate.CreateVariableStatement(U"var", declarations);
}

Expression* Parser::ParsePrimaryExpression() {
  int type = look_ahead->type;
  if (type == TokenType::Identifier) {
    return delegate.CreateIdentifier(Lex());
  }
  if (type == TokenType::StringLiteral || type == TokenType::NumberLiteral ||
      type == TokenType::BooleanLiteral || type == TokenType::NullLiteral) {
    return delegate.CreateLiteral(Lex());
  }
  if (type == TokenType::Keyword) {
    if (IsKeyword(U"this")) {
      Lex();
      return delegate.CreateThisExpression();
    } else if (IsKeyword(U"function")) {
      /* TODO */
      // return ParseFunctionExpression();
    }
  }
  throw runtime_error("Parse primary expression: unknown token.");
}

Identifier* Parser::ParseLiteralProperty() {
  Token* token = Lex();
  if (!IsIdentifierName(token)) {
    throw runtime_error("Unexcepted token.");
  }
  return delegate.CreateIdentifier(token);
}

Expression* Parser::ParseComputedProperty() {
  return ParseExpression();
}

vector<Expression*> Parser::ParseArguments() {
  vector<Expression*> args;
  Token* token;
  while (index < length && !IsPunctuation(U")")) {
    args.push_back(ParseAssignmentExpression());
    token = Lex(); // , or )
    if (((PunctuatorToken*)token)->value == U")")
      break;
    assert(token->type == TokenType::Punctuator &&
           ((PunctuatorToken*)token)->value == U",");
  }
  return args;
}

Expression* Parser::ParsePostfixExpression() {
  SkipUselessness();
  Token* token;
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
    } else if (IsPunctuation(U"(")) {
      Lex(); // (
      vector<Expression*> args = ParseArguments();
      expr = delegate.CreateCallExpression(expr, args);
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
    }  else if (value == U"(")
      return delegate.CreateExpressionStatement(ParseExpression());
  }
  Expression* expr = ParseExpression();
  GetSemicolon();
  return delegate.CreateExpressionStatement(expr);
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

bool VariableDeclarator::Execute() {
  if (init == NULL) {
    current_context->var_table[id->name] = pool.size();
    Object *tmp_obj = new Object(ObjectType::UndefinedObject);
    DEBUG << "undefined_obj: " << (tmp_obj->type) << endl;
    pool.push_back(tmp_obj);
    DEBUG << "Look up undefined " << current_context->LookUp(U"undefined") << endl;
    DEBUG << "Look up " << id->name << " " << current_context->LookUp(id->name) << endl;
  };
  return true;
}

bool VariableStatement::Execute() {
  for (auto declaration: declarations) {
    if (!declaration->Execute()) {
      cout << "ERROR" << endl;
      exit(0);
    }
  }
  return true;
}

bool ExpressionStatement::Execute() {
  /* TODO */
  return true;
}

bool Body::Execute() {
  current_context = new Context(current_context);
  for (auto statement: statements) {
    if (!statement->Execute()) {
      cout << "ERROR" << endl;
      exit(0);
    }
  }
  return true;
}

bool Program::Execute() {
  current_context = global_context;
  if (body != NULL) {
    return body->Execute();
  }
}


} // namespace noop
