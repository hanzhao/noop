#ifndef NOOP_POOL_H
#define NOOP_POOL_H

#include <noop.h>
#include <assert.h>
#include <vector>
#include <string>

namespace noop {

class Object {
public:
  std::string type;
  Object(std::string type): type(type) {}
  virtual ~Object() {}
  template <class T> bool is() {
    return dynamic_cast<T *>(this) != NULL;
  }
  template <class T> T *as() {
    T *t = dynamic_cast<T *>(this);
    return t;
  }
};

class NullObject: public Object {
public:
  NullObject(): Object("null") {}
};

class StringObject: public Object {
public:
  std::string value;
  StringObject(std::string value)
    : Object("string"),
      value(value) {}
};

class NumericObject: public Object {
public:
  double value;
  NumericObject(double value)
    : Object("number"),
      value(value) {}
};

class BooleanObject: public Object {
public:
  bool value;
  BooleanObject(bool value)
    : Object("bool"),
      value(value) {}
};

typedef std::vector<Object *> Pool;
void PoolInit(Pool& pool);
extern Pool pool;
}

#endif
