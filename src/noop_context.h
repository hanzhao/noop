#ifndef NOOP_CONTEXT_H
#define NOOP_CONTEXT_H

namespace noop {
struct Context {
  Context();
  Context(Context* _fa);
  bool InFatherContext(string s);
  bool InContext(string s);
}
} // noop
