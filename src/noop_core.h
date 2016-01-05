#ifndef NOOP_CORE_H
#define NOOP_CORE_H

#include <string>

namespace noop {
namespace Core {

int ExecuteFromFile(std::string script);
int REPL();

} // namespace Core
} // namespace noop

#endif // NOOP_CORE_H
