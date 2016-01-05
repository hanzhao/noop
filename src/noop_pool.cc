#include <noop_pool.h>

#include <iostream>

#include <noop.h>
#include <noop_bindings.h>
#include <noop_parser.h>

using namespace std;

namespace noop {

size_t Object::JumpToProperty(String pro) {
  auto fd = properties.find(pro);
  if (fd != properties.end())
    return fd->second;
  else {
    pool.Add(new UndefinedObject());
  }
}


class Pool {
public:
  VecObj pool;
  Context* global_context;
  Pool(){}
  Pool(Context* global)
  {
    pool_head = &pool;
    Object* undefined_obj = new UndefinedObject();
    Add(undefined_obj);
    /* console.log */
    Object* console = new Object(ObjectType::Object);
    Object* console_log = new NativeFunctionObject(U"log", Bindings::ConsoleLog);
    Object* console_read = new NativeFunctionObject(U"read", Bindings::ConsoleRead);
    console->properties[U"log"] = Add(console_log);
    console->properties[U"read"] = Add(console_read);
    global->var_table[U"console"] = Add(console);
    /* parseFloat */
    Object* parse_float = new NativeFunctionObject(U"parseFloat", Bindings::ParseFloat);
    global->var_table[U"parseFloat"] = Add(parse_float);
    /* parseInt */
    Object* parse_int = new NativeFunctionObject(U"parseInt", Bindings::ParseInt);
    global->var_table[U"parseInt"] = Add(parse_int);
    /* eval */
    Object* eval = new NativeFunctionObject(U"eval", Bindings::Eval);
    global->var_table[U"eval"] = Add(eval);
    global_context = global;
    return;
  }
  size_t Add(Object *objp) {
    pool.push_back(objp);
    mk.push_back(0);
    return pool.size() - 1;
  }
  Object* At(size_t pos) {
    return pool[pos];
  }
  Object* & operator [] (const size_t &pos) {
    return pool[pos];
  }
  void Sweep() {
    for (size_t i = 0; i < pool.size(); ++ i) {
      if (pool[i]->mark == 0) {
        delete pool[i];
        pool[i] = nullptr;
      }
    }
  }
  void GC() {
    for (int i = 0; i < pool.size(); ++ i)
      pool[i]->mark = false;
    Mark(global_context);
    Sweep();
  }
  void Mark(Context* MarkContext) {
    for (auto &x : MarkContext->var_table) {
      int id = x->second;
      MarkObject(id);
    }
    for (int i = 0; i < MarkContext->child.size(); ++ i) {
      Mark(MarkContext->child[i]);
    }
  }
  void MarkObject(size_t id) {
    if (pool[id]->mark == 1)
      return ;
    else
      pool[id]->mark = 1;
    if (pool[id]->type == ObjectType::ArrayObject) {
      for (auto &e: elements) {
        MarkObject(e);
      }
    }
    for (auto &y : pool[id].properties) {
      MarkObject(y.second);
    }
  }

};
Pool pool;
} // noop
