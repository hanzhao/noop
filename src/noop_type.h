#ifndef NOOP_TYPE_H
#define NOOP_TYPE_H

namespace noop {

namespace CharType {

bool IsSpace(Char c);
bool IsDigit(Char c);
bool IsIdentifierHead(Char c);

}

namespace StringType {

bool IsKeyword(String s);

}

}

#endif // NOOP_TYPE_H
