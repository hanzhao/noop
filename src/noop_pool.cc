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
    return pool.Add(new UndefinedObject());
  }
}

Pool pool;
void* pool_head;

Pool::Pool(Context* global) {
  this->pool = new vector<Object*>();
  global_ = global;
  pool_head = this->pool;
  Object* undefined_obj = new UndefinedObject();
  global->var_table[U"undefined"] = Add(undefined_obj);
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
  return;
}

size_t Pool::Add(Object *obj) {
  if (collected.empty()) {
    pool->push_back(obj);
    return pool->size() - 1;
  } else {
    // Reuse
    int idx = collected.front();
    (*pool)[idx] = obj;
    collected.pop();
    return idx;
  }
}

void Pool::Mark(Context* ctx) {
  for (auto &x: ctx->var_table) {
    MarkObject(x.second);
  }
  for (size_t i = 0; i < ctx->child.size(); ++i) {
    Mark(ctx->child[i]);
  }
}

void Pool::MarkObject(size_t id) {
  if (!(*pool)[id] || (*pool)[id]->mark)
    return;
  (*pool)[id]->mark = true;
  if ((*pool)[id]->type == ObjectType::ArrayObject) {
    for (auto &e: ((ArrayObject*)(*pool)[id])->elements) {
      MarkObject(e);
    }
  }
  for (auto &y: (*pool)[id]->properties) {
    MarkObject(y.second);
  }
}

void Pool::Sweep() {
  for (size_t i = 0; i < pool->size(); ++i) {
    if ((*pool)[i] && (*pool)[i]->mark == 0) {
      delete (*pool)[i];
      (*pool)[i] = nullptr;
      collected.push(i);
    }
  }
}

void Pool::CollectGarbage() {
  for (size_t i = 0; i < pool->size(); ++i) {
    if ((*pool)[i]) {
      (*pool)[i]->mark = false;
    }
  }
  Mark(global_);
  Sweep();
}

void Pool::ReportUsage() {
  int count = 0;
  for (size_t i = 0; i < pool->size(); ++i) {
    if ((*pool)[i] != nullptr) {
      ++count;
    }
  }
  DEBUG << "Pool using " << count << ", allocated " << pool->size() << endl;
}

}; // noop
