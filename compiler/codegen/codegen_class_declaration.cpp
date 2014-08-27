#include "codegenerator.hpp"
#include "exceptions.hpp"
#include "../lexer/tokens.hpp"
#include <iostream>

using namespace llvm;

namespace codegen {

  StructType* CodeGenerator::generate(NClassDeclaration& nClassDeclaration) {
    auto name = nClassDeclaration.name.name;
    std::map<std::string, int>* attributeNames = new std::map<std::string, int>();
    std::vector<llvm::Type*> attributeTypes;

    int i = 0;
    for (auto attribute : nClassDeclaration.attributes) {
      attributeNames->insert(std::pair<std::string, int>(attribute->id.name,
                                                    i));
      attributeTypes.push_back(typeOf(&(attribute->type),
                                      getContext().classes));
    }
    auto attributesArrayRef = makeArrayRef(attributeTypes);
    auto type = StructType::create(attributesArrayRef, name);
    getContext().classes[name] = new Class(type, *attributeNames);
    return type;
  }
}
