#include <noop_io.h>

#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

#include <readline/readline.h>
#include <readline/history.h>

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

namespace Console {
/* Read a string and return it.  Returns false on EOF, otherwise true. */
bool ReadLine(string& dest, string prompt) {
  char* line_read = (char *)NULL;
  /* Get a line from the user. */
  line_read = readline(prompt.c_str());
  /* EOF */
  if (line_read == NULL)
    return false;
  /* If the line has any text in it, save it on the history. */
  if (*line_read)
    add_history(line_read);
  /* Save string. */
  dest = line_read;
  free(line_read);
  return true;
}
int InitializeHistory() {
  return read_history(NULL);
}
int SaveHistory() {
  return write_history(NULL);
}
}  // namespace Console

ostream& operator <<(ostream& out, const String u32_str) {
  string u8_str = Encoding::UTF32ToUTF8(u32_str);
  return out << u8_str;
}

ostream& operator <<(ostream& out, const Char u32_chr) {
  String _tmp = U"";
  return out << (_tmp + u32_chr);
}

} // namespace noop
