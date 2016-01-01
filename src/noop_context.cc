#include <noop_parer.h>
#include <hash_map>

namespace noop {
  struct Context {
    Context(){fa = NULL;}
    Context(Context* _fa):fa(_fa){}
    bool InFatherContext(string s) {
        Context *fin = fa;
        while(fin != NULL) {
            if (fin->HashMap[s] != 0) {
                return true;
            }
            fin = fa->fa;
        }
        return false;
    }
    bool InContext(string s) {
      if (HashMap[s] != 0)
        return true;
      else InFatherContext(s);
    }
  };
} // noop
