#include <noop_type.h>

namespace noop {

namespace CharType {

// The following characters are considered whitespace:
// space (U+0020), line feed (U+000A), carriage return (U+000D),
// horizontal tab (U+0009), vertical tab (U+000B), form feed (U+000C)
// and null (U+0000).
bool IsSpace(Char c) {
  return
    (c == 0x0020) || (c == 0x000A) || (c == 0x000D) || (c == 0x0009) ||
    (c == 0x000B) || (c == 0x000C) || (c == 0x0000);
}

bool IsDigit(Char c) {
  return c >= '0' && c <= '9';
}

// This available table is copied from Swift Programming Language
// You can see the detail of it on:
// https://developer.apple.com/library/ios/documentation/Swift/Conceptual/Swift_Programming_Language/LexicalStructure.html
bool IsIdentifierHead(Char c) {
  return
    // identifier-head → Upper- or lowercase letter A through Z
    (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
    // identifier-head → _­
    (c == '_') ||
    // identifier-head → U+00A8, U+00AA, U+00AD, U+00AF, U+00B2–U+00B5, or U+00B7–U+00BA
    (c == 0x00A8) || (c == 0x00AA) || (c == 0x00AD) || (c == 0x00AF) ||
    (c >= 0x00B2 && c <= 0x00B5) || (c >= 0x00B7 && c <= 0x00BA) ||
    // identifier-head → U+00BC–U+00BE, U+00C0–U+00D6, U+00D8–U+00F6, or U+00F8–U+00FF
    (c >= 0x00BC && c <= 0x00BE) || (c >= 0x00C0 && c <= 0x00D6) ||
    (c >= 0x00D8 && c <= 0x00F6) || (c >= 0x00F8 && c <= 0x00FF) ||
    // identifier-head → U+0100–U+02FF, U+0370–U+167F, U+1681–U+180D, or U+180F–U+1DBF
    (c >= 0x0100 && c <= 0x02FF) || (c >= 0x0370 && c <= 0x167F) ||
    (c >= 0x1681 && c <= 0x180D) || (c >= 0x180F && c <= 0x1DBF) ||
    // identifier-head → U+1E00–U+1FFF
    (c >= 0x1E00 && c <= 0x1FFF) ||
    // identifier-head → U+200B–U+200D, U+202A–U+202E, U+203F–U+2040, U+2054, or U+2060–U+206F
    (c >= 0x200B && c <= 0x200D) || (c >= 0x202A && c <= 0x202E) ||
    (c >= 0x203F && c <= 0x2040) || (c == 0x2054) || (c >= 0x2060 && c <= 0x206F) ||
    // identifier-head → U+2070–U+20CF, U+2100–U+218F, U+2460–U+24FF, or U+2776–U+2793
    (c >= 0x2070 && c <= 0x20CF) || (c >= 0x2100 && c <= 0x218F) ||
    (c >= 0x2460 && c <= 0x24FF) || (c >= 0x2776 && c <= 0x2793) ||
    // identifier-head → U+2C00–U+2DFF or U+2E80–U+2FFF
    (c >= 0x2C00 && c <= 0x2DFF) || (c >= 0x2E80 && c <= 0x2FFF) ||
    // identifier-head → U+3004–U+3007, U+3021–U+302F, U+3031–U+303F, or U+3040–U+D7FF
    (c >= 0x3004 && c <= 0x3007) || (c >= 0x3021 && c <= 0x302F) ||
    (c >= 0x3031 && c <= 0x303F) || (c >= 0x3040 && c <= 0xD7FF) ||
    // identifier-head → U+F900–U+FD3D, U+FD40–U+FDCF, U+FDF0–U+FE1F, or U+FE30–U+FE44
    (c >= 0xF900 && c <= 0xFD3D) || (c >= 0xFD40 && c <= 0xFDCF) ||
    (c >= 0xFDF0 && c <= 0xFE1F) || (c >= 0xFE30 && c <= 0xFE44) ||
    // identifier-head → U+FE47–U+FFFD
    (c >= 0xFE47 && c <= 0xFFFD) ||
    // identifier-head → U+10000–U+1FFFD, U+20000–U+2FFFD, U+30000–U+3FFFD, or U+40000–U+4FFFD
    (c >= 0x10000 && c <= 0x1FFFD) || (c >= 0x20000 && c <= 0x2FFFD) ||
    (c >= 0x30000 && c <= 0x3FFFD) || (c >= 0x40000 && c <= 0x4FFFD) ||
    // identifier-head → U+50000–U+5FFFD, U+60000–U+6FFFD, U+70000–U+7FFFD, or U+80000–U+8FFFD
    (c >= 0x50000 && c <= 0x5FFFD) || (c >= 0x60000 && c <= 0x6FFFD) ||
    (c >= 0x70000 && c <= 0x7FFFD) || (c >= 0x80000 && c <= 0x8FFFD) ||
    // identifier-head → U+90000–U+9FFFD, U+A0000–U+AFFFD, U+B0000–U+BFFFD, or U+C0000–U+CFFFD
    (c >= 0x90000 && c <= 0x9FFFD) || (c >= 0xA0000 && c <= 0xAFFFD) ||
    (c >= 0xB0000 && c <= 0xBFFFD) || (c >= 0xC0000 && c <= 0xCFFFD) ||
    // identifier-head → U+D0000–U+DFFFD or U+E0000–U+EFFFD
    (c >= 0xD0000 && c <= 0xDFFF0) || (c >= 0xE0000 && c <= 0xEFFFD);
}

bool IsIdentifier(Char c) {
  return
    // identifier-character → identifier-head
    IsIdentifierHead(c) ||
    // identifier-character → Digit 0 through 9
    (c >= '0' && c <= '9') ||
    // identifier-character → U+0300–U+036F, U+1DC0–U+1DFF, U+20D0–U+20FF, or U+FE20–U+FE2F
    (c >= 0x0300 && c <= 0x036F) || (c >= 0x1DC0 && c <= 0x1DFF) ||
    (c >= 0x20D0 && c <= 0x20FF) || (c >= 0xFE20 && c <= 0xFE2F);
}

} // namespace CharType

namespace StringType {

bool IsKeyword(String s) {
  return
    (s == U"var") ||
    (s == U"if") ||
    (s == U"else") ||
    (s == U"while") ||
    (s == U"this") ||
    (s == U"function") ||
    (s == U"return") ||
    (s == U"typeof") ||
    (s == U"break") ||
    (s == U"continue") ||
    (s == U"arguments");
}

}

} // noop
