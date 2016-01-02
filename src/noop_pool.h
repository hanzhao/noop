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
  UndefinedObject,
  NullObject,
  StringObject,
  NumericObject,
  BooleanObject,
  NaNObject,
  BlackMagicObject
};
}

struct Object {
  int type;
  Object(int type): type(type) {}
};

struct StringObject: Object {
  String value;
  StringObject(String value)
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
