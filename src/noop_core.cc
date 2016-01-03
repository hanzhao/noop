#include <noop_core.h>

#include <fstream>
#include <iostream>

#include <noop.h>
#include <noop_io.h>
#include <noop_parser.h>
#include <noop_pool.h>
#include <noop_context.h>

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
  code = Encoding::UTF8ToUTF32(content);
  DEBUG << "Read code: " << code << endl;
  DEBUG << "Code length: " << code.length() << endl;

  noop::PoolInit(pool);
  DEBUG << pool[0]->type << " " << pool[1]->type << endl;
  DEBUG << "---" << endl;
  global_context->var_table[U"console"] = 1;

  Parser().ParseProgram(code)->Execute();

  /* For DEBUG
  context->var_table[U"undefined"] = 0;

  Context *body = new Context(context);

  body->var_table[U"true"] = 3;

  DEBUG << "Look up undefined " << body->LookUp(U"undefined") << endl;
  DEBUG << "Look up true " << body->LookUp(U"true") << endl;
  DEBUG << "Look up false " << body->LookUp(U"false") << endl;
  */
  return 0;
}

} // Core
} // noop
