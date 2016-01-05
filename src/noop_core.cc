#include <noop_core.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include <noop.h>
#include <noop_context.h>
#include <noop_io.h>
#include <noop_parser.h>
#include <noop_pool.h>

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

  pool = *(new Pool(global_context));
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

int REPL() {
  string content = "";
  String code; // For utf-32
  int ret;
  int seq = 0;
  noop::Console::InitializeHistory();
  pool = *(new Pool(global_context));
  STDOUT << "noop v" << NOOP_VERSION_MAJOR << '.' <<
                       NOOP_VERSION_MINOR << '.' <<
                       NOOP_VERSION_PATCH << endl;
  while (Console::ReadLine(content, "> ")) {
    if (content == ".exit") { break; }
    ret = 0;
    code = Encoding::UTF8ToUTF32(content);
    try {
      ret = Parser().ParseProgram(code)->Execute();
      ++seq;
      string _id = "";
      String str = U"", id = U"";
      stringstream sin;
      sin << "$" << seq;
      sin >> _id;
      id = Encoding::UTF8ToUTF32(_id);
      pool[ret]->ToString(str);
      // $1 = ???
      global_context->var_table[id] = ret;
      // _ = ???
      global_context->var_table[U"$_"] = ret;
      STDOUT << id << " = " << str << endl;
    } catch (runtime_error& e) {
      STDOUT << "Runtime Error: " << e.what() << endl;
    }
  }
  noop::Console::SaveHistory();
  STDOUT << "bye" << endl;
  return 0;
}

int Print(String code) {
  pool = *(new Pool(global_context));
  int ret = Parser().ParseProgram(code)->Execute();
  String str = U"";
  pool[ret]->ToString(str);
  STDOUT << str << endl;
  return 0;
}

int Eval(String code) {
  pool = *(new Pool(global_context));
  Parser().ParseProgram(code)->Execute();
  return 0;
}

} // Core
} // noop
