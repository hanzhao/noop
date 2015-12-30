#include <noop_core.h>

#include <fstream>
#include <iostream>

#include <noop.h>
#include <noop_io.h>

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
  code = noop::Encoding::UTF8ToUTF16(content);
  DEBUG << "Read code: " << code << endl;
  DEBUG << "Code length: " << code.length() << endl;
  // noop::Parser::ParseBody(code);
  return 0;
}

} // Core
} // noop
