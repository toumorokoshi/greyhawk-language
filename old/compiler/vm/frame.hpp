#include <map>
#include <vector>
#include "ops.hpp"
#include "type.hpp"

#ifndef VM_FRAME_HPP
#define VM_FRAME_HPP
namespace VM {


  class GFrame {
  public:
    // globals
    std::map<std::string, int> globalsTable;
    GType** globalsTypes;
    int* indicesInParent;
    int globalsCount;

    // function data
    std::map<std::string, GFunction*> functionByName;
    std::map<int, int> functionTable;
    std::vector<GFunction*> functions;
    int functionCount;

    // locals data actual information about locals are stored in a
    // 'scope' object.
    std::vector<GType*> localsTypes;
    int localsCount;

    GIndex* getGlobal(std::string name);
    GIndex* allocateObject(GType* type);
  };
}
#endif
