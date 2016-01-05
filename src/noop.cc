#include <noop.h>

#include <iostream>
#include <string>
#include <vector>

#include <noop_core.h>
#include <noop_switches.h>

using namespace std;

namespace noop {
  int debug = 0;
  int PrintHelp() {
    cout << "Usage: noop [options] [ --eval script | script.js ]" << endl
                                                                  << endl;
    cout << "Options: " << endl;
    cout << "--eval script     evaluate script" << endl;
    cout << "--print           evaluate script and print result" << endl;
    cout << "--verbose         run in verbose mode and show debug logs" << endl;
    return 0;
  }
  int PrintVersion() {
    cout << "v" << NOOP_VERSION_MAJOR << "."
                << NOOP_VERSION_MINOR << "."
                << NOOP_VERSION_PATCH << endl;
    return 0;
  }
}

int main(int argc, const char* argv[]) {
  vector<string> args;
  for (int i = 1; i < argc; ++i) {
    args.push_back(argv[i]);
  }
  // Not executable options
  for (auto it = args.begin(); it != args.end(); ) {
    if (*it == noop::Switches::kSwitchVerbose) {
      noop::debug = 1;
      it = args.erase(it);
    } else {
      ++it;
    }
  }
  // Executable options
  for (size_t i = 0; i < args.size(); ++i) {
    auto& arg = args[i];
    if (arg == noop::Switches::kSwitchHelp) {
      return noop::PrintHelp();
    }
    if (arg == noop::Switches::kSwitchVersion) {
      return noop::PrintVersion();
    }
    if (i < args.size() - 1 && arg == noop::Switches::kSwitchEval) {
      return noop::Core::Eval(noop::Encoding::UTF8ToUTF32(args[i + 1]));
    } else if (i < args.size() - 1 && arg == noop::Switches::kSwitchPrint) {
      return noop::Core::Print(noop::Encoding::UTF8ToUTF32(args[i + 1]));
    } else if (arg[0] == '-') {
      cout << "noop: bad option: " << arg << endl;
    } else {
      DEBUG << "Execute from file: " << arg << endl;
      return noop::Core::ExecuteFromFile(arg);
    }
  }
  return noop::Core::REPL();
}
