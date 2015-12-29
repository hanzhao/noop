#ifndef NOOP_H
#define NOOP_H
#include <iostream>

namespace noop {

extern int debug;
#define DEBUG if (noop::debug) std::cout << "[DEBUG] "

} // namespace noop

#endif // NOOP_H
