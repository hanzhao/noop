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
    pool.push_back(pool[0]);
    return pool.size() - 1;
  }
}

void* pool_head;
Pool pool;
void PoolInit(Pool& pool, Context* global) {
  Object* undefined_obj = new Object(ObjectType::UndefinedObject);
  pool.push_back(undefined_obj);
  /* console.log */
  Object* console = new Object(ObjectType::Object);
  Object* console_log = new NativeFunctionObject(U"log", Bindings::ConsoleLog);
  Object* console_read = new NativeFunctionObject(U"read", Bindings::ConsoleRead);
  pool.push_back(console_log);
  console->properties[U"log"] = pool.size() - 1;
  pool.push_back(console_read);
  console->properties[U"read"] = pool.size() - 1;
  pool.push_back(console);
  global->var_table[U"console"] = pool.size() - 1;
  return;
}
} // noop
