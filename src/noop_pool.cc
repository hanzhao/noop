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


class Pool {
public:
  VecObj pool;
  Pool(){}
  Pool(Context* global)
  {
    Object* undefined_obj = new UndefinedObject();
    AddToPool(undefined_obj);
    /* console.log */
    Object* console = new Object(ObjectType::Object);
    Object* console_log = new NativeFunctionObject(U"log", Bindings::ConsoleLog);
    Object* console_read = new NativeFunctionObject(U"read", Bindings::ConsoleRead);
    console->properties[U"log"] = AddToPool(console_log);
    console->properties[U"read"] = AddToPool(console_read);
    global->var_table[U"console"] = AddToPool(console);
    /* parseFloat */
    Object* parse_float = new NativeFunctionObject(U"parseFloat", Bindings::ParseFloat);
    global->var_table[U"parseFloat"] = AddToPool(parse_float);
    /* parseInt */
    Object* parse_int = new NativeFunctionObject(U"parseInt", Bindings::ParseInt);
    global->var_table[U"parseInt"] = AddToPool(parse_int);
    /* eval */
    Object* eval = new NativeFunctionObject(U"eval", Bindings::Eval);
    global->var_table[U"eval"] = AddToPool(eval);
    return;
  }
  size_t AddToPool(Object *objp) {
    pool.push_back(objp);
    return pool.size() - 1;
  }
  Object* At(size_t pos) {
    return pool[pos];
  }
};

} // noop
