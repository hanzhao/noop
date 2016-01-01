#ifndef NOOP_CONTEXT_H
#define NOOP_CONTEXT_H

#include <noop.h>
#include <noop_pool.h>

#include <unordered_map>

namespace noop {
struct Context {
  Context *father;
  std::unordered_map<String, size_t> var_table;
  Context(Context* father): father(father) {}
  int LookUp(String s);
};

extern Context *global_context;
} // noop
#endif
