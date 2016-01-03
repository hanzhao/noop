#include <noop_pool.h>
#include <noop_parser.h>
#include <noop.h>

#include <iostream>

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
void PoolInit(Pool& pool) {
  Object* undefined_obj = new Object(ObjectType::UndefinedObject);
  pool.push_back(undefined_obj);
  Object* console = new Object(ObjectType::Object);
  console->properties[U"log"] = 2;
  pool.push_back(console);
  Statement* print = new PrintStatement();
  BlockStatement* block = new BlockStatement();
  block->statements.push_back(print);
  vector<String> params;
  params.push_back(U"data");
  FunctionObject* console_log = new FunctionObject(block, params);
  pool.push_back(console_log);
  /*
  Object *undefined_obj = new Object(ObjectType::UndefinedObject);
  DEBUG << "undefined_obj: " << (undefined_obj->type) << endl;
  pool.push_back(undefined_obj);
  Object *null_obj = new NullObject();
  DEBUG << "null_obj: " << (null_obj->type) << endl;
  pool.push_back(null_obj);
  Object *false_obj = new BooleanObject(false);
  DEBUG << "false_obj: " << (false_obj->type) << endl;
  pool.push_back(false_obj);
  Object *true_obj = new BooleanObject(true);
  DEBUG << "true_obj: " << (true_obj->type) << endl;
  pool.push_back(true_obj);
  */
}
} // noop
