#include <noop_io.h>

#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

using namespace std;

namespace noop {

wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> cvt;

namespace Encoding {
  u16string UTF8ToUTF16(string u8_str) {
    return cvt.from_bytes(u8_str);
  }
  string UTF16ToUTF8(u16string u16_str) {
    return cvt.to_bytes(u16_str);
  }
} // namespace Encoding

ostream& operator <<(ostream& out, const u16string u16_str) {
  string u8_str = Encoding::UTF16ToUTF8(u16_str);
  return out << u8_str;
}

ostream& operator <<(ostream& out, const char16_t u16_chr) {
  u16string u16_str = u"";
  return out << (u16_str + u16_chr);
}

} // namespace noop
