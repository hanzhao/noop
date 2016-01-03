#ifndef NOOP_H
#define NOOP_H
#include <cassert>
#include <codecvt>
#include <iostream>
#include <locale>

namespace noop {

extern int debug;
#define DEBUG if (noop::debug) \
  std::cout << "\033[1m\033[32m[" << __FILE__ << ":" << __LINE__ << "]\033[0m "
#define STDOUT (noop::debug ? \
  std::cout << "\033[1m\033[34m[" << __FILE__ << ":" << __LINE__ << "]\033[0m " : \
  std::cout << "")

typedef std::u32string String;
typedef char32_t Char;
typedef double Number;

} // namespace noop

#endif // NOOP_H
