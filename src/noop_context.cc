#include <noop_context.h>

namespace noop {
Context *global_context = new Context(nullptr);
int Context::LookUp(String s) {
  auto res = var_table.find(s);
  if (res != var_table.end()) {
    return res->second;
  }

  if (father != nullptr) {
    return father->LookUp(s);
  }
  return 0;
}
} // noop
