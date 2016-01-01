#include <noop_parser.h>
#include <noop_context.h>

namespace noop {
int Context::LookUp(String s) {
  auto res = var_table.find(s);
  if (res != var_table.end()) {
    return res->second;
  }

  if (father != NULL) {
    return father->LookUp(s);
  }
  return 0;
}
} // noop
