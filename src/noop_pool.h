#ifndef NOOP_POOL_H
#define NOOP_POOL_H

#include <noop.h>
#include <noop_io.h>
#include <noop_parser.h>

#include <stdexcept>
#include <assert.h>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

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
  BlackMagicObject,
  FunctionObject
};
}

// To solve Object->pool->Object circular reference.
extern void* pool_head;

struct Object {
  int type;
  virtual bool ToNumber(Number& res) { res = 0; return false; }
  virtual bool ToString(String& res) {
    res = U"{ ";
    String value;
    int i = 0;
    for (auto& property: properties) {
      if (i) res += U", ";
      res += property.first;
      res += U": ";
      if (((Object*)(pool_head) + property.second)->ToString(value)) {
        res += value;
      } else {
        return false;
      }
      ++i;
    }
    res += U" }";
    return true;
  }
  std::unordered_map<String, size_t> properties;
  Object(int type): type(type) {}
  size_t JumpToProperty(String pro);
};

struct StringObject: Object {
  String value;
  bool ToNumber(Number& res) override {
    try {
      res = stod(
        Encoding::UTF32ToUTF8(value)
      );
    } catch (const std::invalid_argument& ia) {
      return false;
    }
    return true;
  }
  bool ToString(String& res) override {
    res = value;
    return true;
  }
  StringObject(String value)
    : Object(ObjectType::StringObject),
      value(value) {}
};

struct NumericObject: Object {
  Number value;
  bool ToNumber(Number& res) override {
    res = value;
    return true;
  }
  bool ToString(String& res) override {
    std::string _tmp = "";
    std::stringstream sio(_tmp);
    sio.precision(16);
    sio << value;
    sio >> _tmp;
    res = Encoding::UTF8ToUTF32(_tmp);
    return true;
  }
  NumericObject(Number value)
    : Object(ObjectType::NumericObject),
      value(value) {}
};

struct BooleanObject: Object {
  bool value;
  bool ToNumber(Number& res) override {
    res = value;
    return true;
  }
  bool ToString(String& res) override {
    if (value) {
      res = U"true";
    } else {
      res = U"false";
    }
    return true;
  }
  BooleanObject(bool value): Object(ObjectType::BooleanObject), value(value) { }
};

struct NullObject: Object {
  bool ToNumber(Number& res) override {
    res = 0.0;
    return true;
  }
  bool ToString(String& res) override {
    res = U"null";
    return true;
  }
  NullObject(): Object(ObjectType::NullObject) { }
};

struct NaNObject: Object {
  bool ToNumber(Number& res) override {
    res = 0;
    return true;
  }
  bool ToString(String& res) override {
    res = U"NaN";
    return true;
  }
  NaNObject(): Object(ObjectType::NaNObject) { }
};

struct FunctionObject: Object {
  BlockStatement* func;
  std::vector<String> params;

  FunctionObject(BlockStatement* func, std::vector<String> params)
    : Object(ObjectType::FunctionObject),
      func(func), params(params) {}
};

typedef std::vector<Object*> Pool;
void PoolInit(Pool& pool);
extern Pool pool;

}

#endif
