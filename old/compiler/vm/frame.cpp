#include "frame.hpp"

namespace VM {

  GIndex* GFrame::getGlobal(std::string name) {
    if (globalsTable.find(name) != globalsTable.end()) {
      int index = globalsTable[name];
      return new GIndex {
        .indexType = GLOBAL,
        .registerNum = index,
        .type = globalsTypes[index]
      };
    }
    return NULL;
  }

  GIndex* GFrame::allocateObject(GType* type) {
    localsTypes.push_back(type);
    return new GIndex {
      .registerNum = localsCount++,
      .type = type
    };
  }
}
