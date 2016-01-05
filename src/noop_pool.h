#ifndef NOOP_POOL_H
#define NOOP_POOL_H

#include <cassert>
#include <stdexcept>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <noop.h>
#include <noop_io.h>
#include <noop_parser.h>

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
  FunctionObject,
  ArrayObject,
  NativeFunctionObject
};
}

// To solve Object->pool->Object circular reference.
extern void* pool_head;

struct Object {
  int type;
  virtual bool ToNumber(Number& res) { res = 0; return false; }
  virtual bool ToString(String& res) {
    if (properties.size() == 0) {
      res = U"{}";
      return true;
    }
    res = U"{ ";
    String value;
    int i = 0;
    for (auto& property: properties) {
      if (i) res += U", ";
      res += property.first;
      res += U": ";
      if ((*((((std::vector<Object*>*)pool_head)->begin()) + property.second))->ToString(value)) {
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
  Object(int type):type(type) {}
  size_t JumpToProperty(String pro);
};

struct UndefinedObject: Object {
  bool ToNumber(Number& res) override {
    // Suppress warning
    if (false) { res = 0; }
    return false;
  }
  bool ToString(String& res) override {
    res = U"undefined";
    return true;
  }
  UndefinedObject():Object(ObjectType::UndefinedObject) {}
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
  StringObject(String value):Object(ObjectType::StringObject), value(value) {}
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
  NumericObject(Number value):Object(ObjectType::NumericObject), value(value) {}
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
  BooleanObject(bool value):Object(ObjectType::BooleanObject), value(value) {}
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
  NullObject():Object(ObjectType::NullObject) { }
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
  NaNObject():Object(ObjectType::NaNObject) { }
};

struct FunctionObject: Object {
  String name;
  BlockStatement* function;
  std::vector<String> params;
  bool ToNumber(Number& res) override {
    res = 0;
    return true;
  }
  bool ToString(String& res) override {
    res = U"function " + name + U"() { [code] }";
    return true;
  }
  FunctionObject(String name, BlockStatement* function, std::vector<String> params):Object(ObjectType::FunctionObject), name(name), function(function), params(params) {}
};

struct ArrayObject: Object {
  std::vector<int> elements;
  bool ToNumber(Number& res) override {
    if (false) { res = 0.0; }
    return false;
  }
  bool ToString(String& res) override {
    res = U"[ ";
    String value = U"";
    bool flag = false;
    for (auto& e: elements) {
      if ((*((((std::vector<Object*>*)pool_head)->begin()) + e))->ToString(value)) {
        if (flag) { res += U", "; }
        res += value;
        flag = true;
      } else {
        return false;
      }
    }
    res += U" ]";
    return true;
  }
  ArrayObject(std::vector<int> elements) : Object(ObjectType::ArrayObject), elements(elements) { }
};

struct NativeFunctionObject: Object {
  String name;
  int (*function)(std::vector<Object*>);
  bool ToNumber(Number& res) override {
    res = 0;
    return true;
  }
  bool ToString(String& res) override {
    res = U"function " + name + U"() { [native code] }";
    return true;
  }
  NativeFunctionObject(String name, int (*function)(const std::vector<Object*>)):Object(ObjectType::NativeFunctionObject), name(name), function(function) {}
};

typedef std::vector<Object*> VecObjp;
class Pool {
public:
  VecObjp pool;
  Pool();
  Pool(Context* global);
  size_t AddToPool(Object *objp);
};
Pool* MemPool = NULL;
void* pool_head;
}
#endif
