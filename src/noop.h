#ifndef NOOP_H
#define NOOP_H
#include <iostream>

namespace noop {

extern int debug;
#define DEBUG if (noop::debug) \
  std::cout << "\033[1m\033[32m[DEBUG]\033[0m "

} // namespace noop

#endif // NOOP_H
