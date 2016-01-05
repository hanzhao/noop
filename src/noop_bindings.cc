#include <noop_bindings.h>

#include <iostream>

#include <noop.h>
#include <noop_io.h>
#include <noop_pool.h>
#include <noop_parser.h>

using namespace std;

namespace noop {
namespace Bindings {

int ConsoleLog(const std::vector<Object*> args) {
  String str;
  for (size_t i = 0; i < args.size(); ++i) {
    if (args[i]->ToString(str)) {
      if (i == args.size() - 1)
        STDOUT << str << endl;
      else
        STDOUT << str;
    }
  }
  return 0;
}

int ConsoleRead(const std::vector<Object*> args) {
  // Suppress unused warning
  if (false) { args.size(); }
  string input;
  getline(cin, input);
  StringObject* obj = new StringObject(Encoding::UTF8ToUTF32(input));
  return pool.Add(obj);
}

int ParseFloat(const std::vector<Object*> args) {
  String str = U"";
  if (args.size() == 0 || !(args[0]->ToString(str))) {
    NaNObject* nan = new NaNObject();
    return pool.Add(nan);
  }
  Number num;
  stringstream sin(Encoding::UTF32ToUTF8(str));
  sin >> num;
  NumericObject* obj = new NumericObject(num);
  return pool.Add(obj);
}

int ParseInt(const std::vector<Object*> args) {
  String str = U"";
  if (args.size() == 0 || !(args[0]->ToString(str))) {
    NaNObject* nan = new NaNObject();
    return pool.Add(nan);
  }
  Number num;
  stringstream sin(Encoding::UTF32ToUTF8(str));
  sin >> num;
  NumericObject* obj = new NumericObject((Number)((long long)num));
  return pool.Add(obj);
}

int Eval(const std::vector<Object*> args) {
  if (args.size() == 0) return 0;
  String code = U"";
  if (args[0]->ToString(code)) {
    return Parser().ParseProgram(code)->Execute();
  } else
    return 0;
}

} // namespace Binding
} // namespace noop
