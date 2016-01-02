#include <noop_binding.h>

#include <iostream>

#include <noop_io.h>
#include <noop_pool.h>

using namespace std;

namespace noop {
namespace Binding {

int readline() {
  string input;
  getline(cin, input);
  StringObject* obj = new StringObject(Encoding::UTF32ToUTF8(input));
  pool.push_back(obj);
  return pool.size() - 1;
}

int parseFloat(int index) {
  Number num;
  stringstream sin(Encoding::UTF32ToUTF8(((StringObject*)pool[index])->value));
  sin >> num;
  NumericObject* obj = new NumericObject(num);
  pool.push_back(obj);
  return pool.size() - 1;
}

int parseInt(int index) {
  Number num;
  stringstream sin(Encoding::UTF32ToUTF8(((StringObject*)pool[index])->value));
  sin >> num;
  NumericObject* obj = new NumericObject((Number)((long long)num));
  pool.push_back(obj);
  return pool.size() - 1;
}

} // namespace Binding
} // namespace noop
