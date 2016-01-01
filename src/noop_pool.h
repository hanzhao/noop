#ifndef NOOP_POOL_H
#define NOOP_POOL_H

#include <noop.h>
#include <assert.h>
#include <vector>
#include <string>

namespace noop {

namespace ObjectType {
enum {
  Object,
  NullObject,
  StringObject,
  NumericObject,
  BooleanObject,
  UndefinedObject
};
}

struct Object {
  int type;
  Object(int type): type(type) {}
  template <class T> bool is() {
    return dynamic_cast<T*>(this) != NULL;
  }
  template <class T> T* as() {
    T* t = dynamic_cast<T *>(this);
    return t;
  }
};

struct NullObject: Object {
  NullObject(): Object(ObjectType::NullObject) {}
};

struct StringObject: Object {
  std::string value;
  StringObject(std::string value)
    : Object(ObjectType::StringObject),
      value(value) {}
};

struct NumericObject: Object {
  double value;
  NumericObject(double value)
    : Object(ObjectType::NumericObject),
      value(value) {}
};

struct BooleanObject: Object {
  bool value;
  BooleanObject(bool value)
    : Object(ObjectType::BooleanObject),
      value(value) {}
};

typedef std::vector<Object*> Pool;
void PoolInit(Pool& pool);
extern Pool pool;
}

#endif
