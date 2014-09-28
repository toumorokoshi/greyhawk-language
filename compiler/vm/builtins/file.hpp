#include "../class.hpp"
#include "../object.hpp"
#include "../basic_types/string.hpp"
#include <fstream>

#ifndef VM_BUILTINS_FILE_HPP
#define VM_BUILTINS_FILE_HPP

namespace VM {

  VMClass* getVMFileClass();
  VMObject* _getVMStdin();

  class VMFile : public VMObject {
  public:
    std::istream& stream;
    VMFile(std::istream& _stream) :
      stream(_stream) {}

    virtual VMClass* getType() { return getVMFileClass(); }

    VMString* nextLine() {
      std::string buffer;
      std::getline(stream, buffer);
      return new VMString(buffer);
    }

    VMObject* call(std::string methodName, std::vector<VMObject*>&) {
      if (methodName == "nextLine") { return nextLine(); }

      throw VMException("File has no method " + methodName);
    }

  };
}

#endif
