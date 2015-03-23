#include <map>
#include <vector>
#include "ops.hpp"
#include "type.hpp"
#include "object.hpp"

#ifndef VM_CONTEXT_HPP
#define VM_CONTEXT_HPP

namespace VM {

  struct GScopeInstance;

  // we use a class instead of a struct
  // so we can encapsulate things for now,
  // until a good mechanism is decided.
  class GScope {
  public:
    // globals data
    GValue** globals;
    std::map<std::string, int> globalsTable;
    std::vector<GType*> globalsTypes;
    int globalsCount;

    // locals data
    std::map<std::string, int> localsTable;
    std::vector<GType*> localsTypes;
    int localsCount;

    // get the index of an object
    // negative = global scope (GLOBAL_LOAD)
    // positive = local scope (LOAD)
    GIndex* getObject(std::string name) {
      if (localsTable.find(name) != localsTable.end()) {
        int index = localsTable[name];
        return new GIndex{
          .registerNum = index,
          .type = localsTypes[index]
        };
      }

      if (globalsTable.find(name) != globalsTable.end()) {
        int index = -globalsTable[name];
        return new GIndex {
          .registerNum = index,
          .type = globalsTypes[index]
        };
      }

      return NULL;
    }

    GIndex* addObject(std::string name, GType* type) {
      GIndex* index = allocateObject(type);
      localsTable[name] = index->registerNum;
      return index;
    }

    GIndex* allocateObject(GType* type) {
      localsTypes.push_back(type);
      return new GIndex {
        .registerNum = localsCount++,
        .type = type
      };
    }
    GScopeInstance createInstance();
  };

  struct GScopeInstance {
    GScope* scope;
    GValue* values;

    GValue getValue(std::string);

    GScope* createChildScope();
  };

}
#endif
