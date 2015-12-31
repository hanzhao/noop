#include <noop_core.h>

#include <fstream>
#include <iostream>

#include <noop.h>
#include <noop_io.h>
#include <noop_parser.h>
#include <noop_type.h>

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
  String code; // We need to use utf-32 inside.
  bool flag = false;
  /* Read entire file and standardize EOL */
  while (getline(fin, buff)) {
    if (flag) content += "\n";
    flag = true;
    content += buff;
  }
  /* Convert code from UTF-8 to UTF-32 */
  code = noop::Encoding::UTF8ToUTF32(content);
  noop::Parser::InitData(code);
  noop::Parser::GetNextToken();
  while (true) {
    switch (Parser::current_token) {
      case TOKEN::VAR_DECLARATION:
        noop::Parser::HandleVarDeclaration();
        break;
      default:
        for (unsigned long i = 0; i < Parser::body.size(); ++i) {
          DEBUG << "Body: " << i << endl;
          Parser::body[i]->Print();
        }
        return 0;
    }
  }
  DEBUG << "Read code: " << code << endl;
  DEBUG << "Code length: " << code.length() << endl;
  // noop::Parser::ParseBody(code);
  return 0;
}

} // Core
} // noop
