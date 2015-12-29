#include <noop_core.h>

#include <codecvt>
#include <iostream>
#include <fstream>
#include <locale>

#include <noop.h>

using namespace std;

namespace noop {
namespace Core {

int ExecuteFromFile(string script) {
  ifstream fin(script);
  /* File is not found */
  if (!fin.is_open()) {
    cout << "Error: Cannot find script " << script << endl;
    return -1;
  }
  string content = "", buff;
  u16string code; // We need to use utf-16 inside.
  bool flag = false;
  /* Read entire file and standardize EOL */
  while (getline(fin, buff)) {
    if (flag) content += "\n";
    flag = true;
    content += buff;
  }
  /* Convert code from UTF-8 to UTF-16 */
  wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> cvt;
  code = cvt.from_bytes(content);
  DEBUG << "Code length: " << code.length() << endl;
  noop::Parser::ParseStatement(code);
  return 0;
}

} // Core
} // noop
