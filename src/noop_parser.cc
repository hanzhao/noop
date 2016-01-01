#include <noop_parser.h>

/* LL(1) parser for simplified JavaScript. */

namespace noop {

Token::Token(int type, int start, int end)
    : type(type), start(start), end(end) {
}

void SkipUselessness() {
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
      index += 1
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

/*
 * Token Getter
 */
StringLiteralToken* GetStringLiteral() {
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
    else if (ch == '\\') {
      ch = source[index++];
      switch (ch) {
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

NumericLiteralToken* GetNumericLiteral() {
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
  streamstring(Encoding::UTF32ToUTF8(str)) >> token->value;
  return token;
}

IdentifierToken* GetIdentifier() {
  String str = U"";
  int start = index, c;
  while (index < length) {
    c = source[index++];
    if (!CharType::IsIdentifier(c))
      break;
    str += c;
  }
  IdentifierToken* token = new IdentifierToken(TokenType::Identifier,
                                               start, index);
  token->name = str;
  return token;
}

PunctuatorToken* GetPunctuator() {
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
  if (c2 == '=' && c3 == '=' && (c1 == '!' || c1 == '=')) {
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
  if (Char::IsDigit(c)) {
    return GetNumericLiteral();
  }
  if (Char::IsIdentifier(c)) {
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
Token* Lex() {
  Token* token = look_ahead;
  index = token->start;
  look_ahead = LookAhead();
  index = token->start;
  return token;
}
/*
 * Parse an entire JavaScript source code
 */
Program* Parser::ParseProgram(String code) {
  source = code;
  index = 0;
  length = code.length();
  look_ahead = NULL;
  Peek();
  return ParseBody();
}

} // namespace noop
