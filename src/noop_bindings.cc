#include <noop_bindings.h>

#include <iostream>

#include <noop.h>
#include <noop_io.h>
#include <noop_pool.h>

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
  pool.push_back(obj);
  return pool.size() - 1;
}

int ParseFloat(const std::vector<Object*> args) {
  if (args.size() == 0) {
    NaNObject* nan = new NaNObject();
    pool.push_back(nan);
    return pool.size() - 1;
  }
  Number num;
  stringstream sin(Encoding::UTF32ToUTF8(((StringObject*)args[0])->value));
  sin >> num;
  NumericObject* obj = new NumericObject(num);
  pool.push_back(obj);
  return pool.size() - 1;
}

int ParseInt(const std::vector<Object*> args) {
  if (args.size() == 0) {
    NaNObject* nan = new NaNObject();
    pool.push_back(nan);
    return pool.size() - 1;
  }
  Number num;
  stringstream sin(Encoding::UTF32ToUTF8(((StringObject*)args[0])->value));
  sin >> num;
  NumericObject* obj = new NumericObject((Number)((long long)num));
  pool.push_back(obj);
  return pool.size() - 1;
}

} // namespace Binding
} // namespace noop
