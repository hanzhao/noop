#include <noop_io.h>

#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

using namespace std;

namespace noop {

wstring_convert<codecvt_utf8<Char>, Char> cvtDelegate;

namespace Encoding {
  String UTF8ToUTF32(string u8_str) {
    return cvtDelegate.from_bytes(u8_str);
  }
  string UTF32ToUTF8(String u32_str) {
    return cvtDelegate.to_bytes(u32_str);
  }
} // namespace Encoding

ostream& operator <<(ostream& out, const String u32_str) {
  string u8_str = Encoding::UTF32ToUTF8(u32_str);
  return out << u8_str;
}

ostream& operator <<(ostream& out, const Char u32_chr) {
  String _tmp = U"";
  return out << (_tmp + u32_chr);
}

} // namespace noop
