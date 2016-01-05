#ifndef NOOP_CONTEXT_H
#define NOOP_CONTEXT_H

#include <noop.h>
#include <vector>
#include <unordered_map>

namespace noop {
struct Context {
  Context *father;
  std::unordered_map<String, size_t> var_table;
  std::vector<Context*> child;
  Context(Context* father): father(father) {
    father->child.push_back(this);
  }
  int LookUp(String s);
  void AddChild(Context* ch) {
    child.push_back(ch);
  }
  ~Context(){
    father->child.pop_back();
  }
};

extern Context *global_context;
} // noop
#endif
