#ifndef NOOP_CORE_H
#define NOOP_CORE_H

#include <string>

#include <noop_io.h>

namespace noop {
namespace Core {

int ExecuteFromFile(std::string script);
int REPL();
int Eval(String code);
int Print(String code);

} // namespace Core
} // namespace noop

#endif // NOOP_CORE_H
