#ifndef NOOP_POOL_H
#define NOOP_POOL_H

#include <noop.h>
#include <noop_io.h>

#include <stdexcept>
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
  virtual bool ToNumber(Number& res) { res = 0; return false; };
  virtual bool ToString(String& res) { res = U""; return false; };
  Object(int type): type(type) {}
};

struct StringObject: Object {
  String value;
  bool ToNumber(Number& res) {
    try {
      res = stod(
        Encoding::UTF32ToUTF8(value)
      );
    } catch (const std::invalid_argument& ia) {
      return false;
    };
    return true;
  };
  bool ToString(String& res) {
    res = value;
    return true;
  };
  StringObject(String value)
    : Object(ObjectType::StringObject),
      value(value) {}
};

struct NumericObject: Object {
  Number value;
  bool ToNumber(Number& res) {
    res = value;
    return true;
  };
  bool ToString(String& res) {
    if (value == (int)(value)) {
      res = Encoding::UTF8ToUTF32(std::to_string((int)(value)));
    } else {
      res = Encoding::UTF8ToUTF32(std::to_string(value));
    }
    return true;
  };
  NumericObject(double value)
    : Object(ObjectType::NumericObject),
      value(value) {}
};

struct BooleanObject: Object {
  bool value;
  bool ToNumber(Number& res) {
    res = value;
    return true;
  };
  bool ToString(String& res) {
    if (value) {
      res = U"true";
    } else {
      res = U"false";
    }
    return true;
  };
  BooleanObject(bool value)
    : Object(ObjectType::BooleanObject),
      value(value) {}
};

typedef std::vector<Object*> Pool;
void PoolInit(Pool& pool);
extern Pool pool;
}

#endif
