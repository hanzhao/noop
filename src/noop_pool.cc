#include <noop_pool.h>
#include <noop.h>

#include <iostream>

using namespace std;

namespace noop {
Pool pool;
void PoolInit(Pool& pool) {
  Object *undefined_obj = new Object(ObjectType::UndefinedObject);
  pool.push_back(undefined_obj);
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
