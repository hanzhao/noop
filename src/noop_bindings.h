#ifndef NOOP_BINDING_H
#define NOOP_BINDING_H

#include <vector>

#include <noop_pool.h>

namespace noop {
namespace Bindings {

int ParseFloat(int ObjectIndex);
int ConsoleLog(const std::vector<Object*> args);
int ConsoleRead(const std::vector<Object*> args);

}
}

#endif // NOOP_BINDING_H
