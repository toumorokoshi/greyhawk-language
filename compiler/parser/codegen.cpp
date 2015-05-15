#include "nodes.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <typeinfo>

#ifdef DEBUG
  #define debug(s) std::cerr << s << std::endl;
#else
  #define debug(s);
#endif

using namespace VM;
using namespace lexer;
using namespace codegen;

namespace parser {

  GIndex* enforceLocal(GScope* scope, GIndex* value,
                       GInstructionVector& instructions) {
    if (value->indexType == LOCAL) {
      return value;
    }

    auto index = scope->allocateObject(value->type);

    switch (value->indexType) {
    case GLOBAL:
      instructions.push_back({
          GLOBAL_LOAD, new GOPARG[2] { index->registerNum, value->registerNum}
      });
      break;

    case OBJECT_PROPERTY:
      instructions.push_back({
          INSTANCE_LOAD_ATTRIBUTE, new GOPARG[3] {
            index->registerNum,
            value->objectIndex->registerNum,
            value->registerNum
          }
      });
      break;

    default:
      throw ParserException("unable to localize variable");
    }

    return index;
  }

  // determine if the identifier is a type
  bool isType(std::string identifier) {
    return identifier[0] >= 'A' && identifier[0] <= 'Z';
  }

  GType* evaluateType(std::string typeName) {
    if (typeName[0] <= 'A' || typeName[0] >= 'Z') {
      throw ParserException(typeName + " is not a class!");
    }

    if (typeName == "Int") { return getInt32Type(); }
    else if (typeName == "Bool") { return getBoolType(); }
    else if (typeName == "String") { return getStringType(); }
    else if (typeName == "Char") { return getCharType(); }
    else if (typeName == "None") { return getNoneType(); }

    throw ParserException("Cannot find class " + typeName);
  }

  GInstruction* generateRoot(VM::GEnvironment* environment, PBlock* block) {
    auto scope = new GScope { .environment = environment };
    auto instructions = block->generate(scope);

    instructions->push_back(GInstruction { END, NULL });
    return &(*instructions)[0];
  }

  GInstructionVector* PBlock::generate(GScope* scope) {
    debug("PARSER: Generating Block")
    auto instructions = new GInstructionVector;
    for (auto statement : statements) {
      debug("  generating statement...")
      statement->generateStatement(scope, *instructions);
    }
    debug("  finalizing...");
    scope->finalize();
    debug("  finished finalizing...");
    return instructions;
  }

  GIndex* PCall::generateExpression(GScope* scope,
                                     GInstructionVector& instructions) {
    debug("  calling method");
    if (name == "print") {

      debug("    adding print.");
      auto argument = arguments[0]->generateExpression(scope, instructions);
      debug("    enforcing local on print...")
      argument = enforceLocal(scope, argument, instructions);
      debug("    after enforcing local on print...")
      GOPCODE op;
      auto type = argument->type;
      if (type == getStringType()) {
        op = GOPCODE::PRINT_STRING;
      } else if (type == getInt32Type()) {
        op = GOPCODE::PRINT_INT;
      } else if (type == getCharType()) {
        op = GOPCODE::PRINT_CHAR;
      } else if (type == getFloatType()) {
        op = GOPCODE::PRINT_FLOAT;
      } else {
        if (type == NULL) {
          throw ParserException("There was an issue with the compiler! Recieved null class type.");
        }
        throw ParserException("Unable to print class " + type->name);
      }
      debug(op)

      instructions.push_back(GInstruction {
          op, new GOPARG[1] { { argument->registerNum } }
      });

    } else if (auto functionIndex = scope->getObject(name)) {
      functionIndex = enforceLocal(scope, functionIndex, instructions);

      GOPCODE instruction;
      GType* returnType;

      debug("    getting type values");
      debug(name)
      if (isType(name)) {
        instruction = INSTANCE_CREATE;
        returnType = scope->getClass(name);
      } else {
        instruction = FUNCTION_CALL;
        returnType = scope->getFunction(name)->returnType;
      }
      if (functionIndex->type != getFunctionType() && functionIndex->type != getClassType()) {
        throw ParserException("" + name + " is not a Function or Class! found " + functionIndex->type->name);
      }

      auto opArgs = new std::vector<GOPARG>;

      debug("allocating object");
      // first OPARG is the return object
      auto returnObject = scope->allocateObject(returnType);
      opArgs->push_back(GOPARG{ returnObject->registerNum });

      // second OPARG is the function pointer
      opArgs->push_back(GOPARG{ .registerNum = functionIndex->registerNum });

      debug("allocating arguments");
      // third on are the actual arguments
      for (auto argument : arguments) {
        auto object = argument->generateExpression(scope, instructions);
        object = enforceLocal(scope, object, instructions);
        opArgs->push_back(GOPARG { object->registerNum });
      }

      debug("finishing function");
      debug(instruction);
      debug(&(*opArgs)[0]);
      debug(opArgs);
      instructions.push_back(GInstruction { instruction, &(*opArgs)[0] });
      return returnObject;
    } else {
      throw ParserException("Unable to call method " + name);
    }
    debug("returning null on function call")
    return NULL;
  }

  void PDeclare::generateStatement(GScope* scope,
                                   GInstructionVector& instructions) {
    debug("  declaring...")
    auto value = expression->generateExpression(scope, instructions);
    auto newVar = scope->addObject(name, value->type);
    instructions.push_back(GInstruction {
        SET, new GOPARG[2] { {value->registerNum}, {newVar->registerNum} }
    });
  }

  void setArrayElement(GScope* scope, GInstructionVector& instructions,
                       PArrayAccess* arrayAccess, GIndex* value) {
    auto array = arrayAccess->value->generateExpression(scope, instructions);
    auto index = arrayAccess->index->generateExpression(scope, instructions);
    instructions.push_back(GInstruction { ARRAY_SET_VALUE, new GOPARG[3] {
          {array->registerNum}, {index->registerNum}, {value->registerNum}
    }});
  }

  void PAssign::generateStatement(GScope* scope,
                                  GInstructionVector& instructions) {
    // if the value is an array, we set it differently
    auto value = expression->generateExpression(scope, instructions);

    if (auto arrayAccess = dynamic_cast<PArrayAccess*>(identifier)) {
      setArrayElement(scope, instructions, arrayAccess, value);

    } else {
      auto ident = identifier->generateExpression(scope, instructions);

      if (ident->type != value->type) {
        throw ParserException("type mismatch in assignment!");
      }

      switch (ident->indexType) {
      case LOCAL:
        instructions.push_back(GInstruction {
            SET, new GOPARG[2] { {value->registerNum}, {ident->registerNum}}
        });
        break;
      case GLOBAL:
        instructions.push_back(GInstruction {
            GLOBAL_SET, new GOPARG[2] {{ident->registerNum}, {value->registerNum}}
        });
        break;
      case OBJECT_PROPERTY:
        instructions.push_back({
          INSTANCE_SET_ATTRIBUTE, new GOPARG[3] {
            ident->objectIndex->registerNum,
            ident->registerNum,
            value->registerNum,
          }
        });
        break;
      }


      if (ident->indexType == GLOBAL) {
      } else {
      }

    }
  }

  /* GIndex* intToFloat(GIndex* integer, GInstructionVector& instructions) {
    auto castResult = new GObject { getFloatType(), {0}};
    instructions.push_back(GInstruction {
        GOPCODE::INT_TO_FLOAT, new GIndex*[2] { integer, castResult }
      });
    return castResult;
    } */

  GIndex* PBinaryOperation::generateExpression(GScope* scope,
                                                GInstructionVector& instructions) {
    debug("PARSER: PBinaryOperation");
    auto lhsObject = lhs->generateExpression(scope, instructions);
    auto rhsObject = rhs->generateExpression(scope, instructions);

    lhsObject = enforceLocal(scope, lhsObject, instructions);
    rhsObject = enforceLocal(scope, rhsObject, instructions);

    // cast as necessary
    /* if (lhsObject->type == getFloatType() && rhsObject->type == getInt32Type()) {
      rhsObject = intToFloat(rhsObject, instructions);
    }
    if (lhsObject->type == getInt32Type() && rhsObject->type == getFloatType()) {
      lhsObject = intToFloat(lhsObject, instructions);
      } */

    if (lhsObject->type != rhsObject->type) {
      throw ParserException("type mismatch during binary operation! " +
                            lhsObject->type->name + " and " +
                            rhsObject->type->name);
    }

    GIndex* resultObject;
    VM::GOPCODE opCode;

    switch (op) {
    case L::EQUAL:
      debug("  equal");
      debug(lhsObject->type->name);
      resultObject = scope->allocateObject(getBoolType());
      if (lhsObject->type == getFloatType()) {
        opCode = GOPCODE::FLOAT_EQ;
        break;
      } else if (lhsObject->type == getCharType()) {
        debug("  char eq");
        opCode = GOPCODE::CHAR_EQ;
        break;
      }

    case L::LESS_THAN:
      debug("  less than");
      resultObject = scope->allocateObject(getBoolType());
      opCode = GOPCODE::LESS_THAN_INT;
      break;

    case L::PLUS: {
      if (lhsObject->type == getFloatType()) {
        resultObject = scope->allocateObject(getFloatType());
        opCode = GOPCODE::ADD_FLOAT;
        break;

      } else if (lhsObject->type == getInt32Type()) {
        resultObject = scope->allocateObject(getInt32Type());
        opCode = GOPCODE::ADD_INT;
        break;
      }
    }

    case L::MINUS: {
      if (lhsObject->type == getFloatType()) {
        resultObject = scope->allocateObject(getFloatType());
        opCode = GOPCODE::SUBTRACT_FLOAT;
        break;

      } else if (lhsObject->type == getInt32Type()) {
        resultObject = scope->allocateObject(getInt32Type());
        opCode = GOPCODE::SUBTRACT_INT;
        break;
      }
    }

    case L::MUL: {
      if (lhsObject->type == getFloatType()) {
        resultObject = scope->allocateObject(getFloatType());
        opCode = GOPCODE::MULTIPLY_FLOAT;
        break;

      } else if (lhsObject->type == getInt32Type()) {
        resultObject = scope->allocateObject(getInt32Type());
        opCode = GOPCODE::MULTIPLY_INT;
        break;
      }
    }

    case L::DIV: {
      if (lhsObject->type == getFloatType()) {
        resultObject = scope->allocateObject(getFloatType());
        opCode = GOPCODE::DIVIDE_FLOAT;
        break;

      } else if (lhsObject->type == getInt32Type()) {
        resultObject = scope->allocateObject(getInt32Type());
        opCode = GOPCODE::DIVIDE_INT;
        break;
      }
    }

    case L::OR: {
      resultObject = scope->allocateObject(getBoolType());
      opCode = GOPCODE::INT_OR;
      break;
    }

    default:
      throw ParserException("binary op not implemented: " + lexer::tokenMap[op]);
    }

    instructions.push_back(GInstruction { opCode, new GOPARG[3] {
          lhsObject->registerNum, rhsObject->registerNum, resultObject->registerNum }
      });
    return resultObject;
  }

  void PIncrement::generateStatement(GScope* scope,
                                     GInstructionVector& instructions) {
    auto toIncrement = identifier->generateExpression(scope, instructions);
    if (toIncrement->type != getInt32Type()) {
      throw ParserException("only supporting int32 for increment ATM");
    }

    auto incrementer = expression->generateExpression(scope, instructions);
    if (incrementer->type != getInt32Type()) {
      throw ParserException("only supporting int32 for increment ATM");
    }

    instructions.push_back(GInstruction{
        GOPCODE::ADD_INT, new GOPARG[3] {
          toIncrement->registerNum, incrementer->registerNum, toIncrement->registerNum
            }
      });
  }

  void PReturn::generateStatement(GScope* scope,
                                  GInstructionVector& instructions) {
    auto returnObject = expression->generateExpression(scope, instructions);
    instructions.push_back(GInstruction {
        GOPCODE::RETURN, new GOPARG[1] { { returnObject->registerNum }}
    });
  }

  GIndex* PConstantString::generateExpression(GScope* s, GInstructionVector& i) {
    auto target = s->allocateObject(getStringType());
    i.push_back(GInstruction {
        GOPCODE::LOAD_CONSTANT_STRING, new VM::GOPARG[2] {
          { target->registerNum }, GOPARG { .asString = value.c_str() }
        }});
    return target;
  }

  GIndex* PIdentifier::generateExpression(GScope* scope,
                                          GInstructionVector& instructions) {
    debug("PARSER: PIdentifier");

    auto debugScope = scope;
    while (debugScope != NULL) {
      debug("  locals:");
      for (auto& kv: debugScope->localsByName) {
        debug("    " << kv.first << ": " << kv.second);
      }
      debugScope = debugScope->parentScope;
    }

    debug("  environment locals:");
    for (auto& kv: scope->environment->localsByName) {
      debug("    " << kv.first << ": " << kv.second);
    }

    debug("  environment globals:");
    for (auto& kv: scope->environment->globalsByName) {
      debug("    " << kv.first << ": " << kv.second);
    }

    auto object = scope->getObject(name);

    if (object == NULL) {
      throw ParserException("Object " + name + " is not defined in this scope!");
    }
    return object;
  }

  void PForLoop::generateStatement(GScope* scope,
                                   GInstructionVector& instructions) {
    initializer->generateStatement(scope, instructions);
    auto statements = body->generate(scope);
    auto forLoopStart = instructions.size();
    instructions.reserve(instructions.size() + distance(statements->begin(), statements->end()));
    instructions.insert(instructions.end(), statements->begin(), statements->end());
    incrementer->generateStatement(scope, instructions);
    auto conditionObject = condition->generateExpression(scope, instructions);
    instructions.push_back(GInstruction {
        GOPCODE::BRANCH,
          new GOPARG[3] { conditionObject->registerNum, (int) forLoopStart - ((int) instructions.size()), 1 }
      });
  }

  void PFunctionDeclaration::generateStatement(GScope* scope,
                                               GInstructionVector& instructions) {
    debug("PFunctionDeclaration");

    if (scope->getObject(name) != NULL) {
      throw ParserException("Cannot redeclare " + name);
    }

    debug("  return type is: " + returnType)
    auto index = scope->addFunction(name, new GFunction {
        .argumentCount = (int) arguments.size(),
        .returnType = evaluateType(returnType),
    }, this);
    debug("PFunctionDeclaration name: " << name);
    auto functionIndex = scope->functionsByName[name];
    debug("PFunctionDeclaration index: " << functionIndex);

    instructions.push_back(GInstruction {
        GOPCODE::FUNCTION_CREATE, new VM::GOPARG[2] {
          { index->registerNum }, { functionIndex }
        }});
  }

  void PFunctionDeclaration::generateBody(GFunction* function, GScope* scope) {
    debug("generating function body");
    GScope* functionScope = scope->createChild(true);

    function->argumentNames = new std::string[arguments.size()];
    function->argumentTypes = new GType*[arguments.size()];
    function->environment = functionScope->environment;


    int i = 0;
    for (auto argument : arguments) {
      debug(i);
      auto type = evaluateType(argument->second);

      functionScope->addObject(argument->first, type);
      function->argumentNames[i] = argument->first;
      function->argumentTypes[i] = type;
      i++;
    }

    auto vmBody = body->generate(functionScope);
    vmBody->push_back(GInstruction { END, 0 });
    function->instructions = &(*vmBody)[0];
    debug("function instructions: " << function->instructions);
    debug("function: " << function);
  }

  void PClassDeclaration::generateStatement(GScope* scope,
                                            GInstructionVector& instr) {
    debug("  creating class " + name);
    debug(scope);
    debug(scope->environment);
    auto classScope = scope->createChild(true);
    debug ("    finished creating child.")

    for (auto& kv: attributes) {
      classScope->addObject(kv.first, evaluateType(kv.second));
    }

    debug("    creating class attributes")
    GFunction* createdFunctions[methods.size()];

    for (int i = 0; i < (int) methods.size(); i++) {
      auto method = methods[i];
      auto function = new GFunction {
        .argumentCount = (int) method->arguments.size(),
        .returnType = evaluateType(method->returnType)
      };
      createdFunctions[i] = function;
      debug("    adding function " + method->name + "...")
      classScope->addFunction(method->name, function, method);
    }

    // we generate the bodies at the end, to ensure that all
    // class functions are available to all other methods.
    for (int i = 0; i < (int) methods.size(); i++) {
      auto method = methods[i];
      auto function = createdFunctions[i];
      method->generateBody(function, classScope);
    }

    auto type = new GType {
      .name = name,
      .attributeCount = (int) attributes.size(),
      .environment = classScope->environment,
      .functionCount = (int) methods.size()
    };

    auto classIndex = scope->addClass(name, type);
    auto classInLocalsIndex = scope->addObject(name, getClassType());
    instr.push_back(GInstruction {
        GOPCODE::TYPE_LOAD, new VM::GOPARG[2] {
          classInLocalsIndex->registerNum, classIndex->registerNum
        }
    });
  }

  void PIfElse::generateStatement(GScope* scope,
                                  GInstructionVector& instructions) {
    debug("PIfElse");
    auto conditionObject = condition->generateExpression(scope, instructions);

    auto trueScope = scope->createChild(false);
    auto trueInstructions = trueBlock->generate(trueScope);

    instructions.push_back(GInstruction { GOPCODE::BRANCH, new GOPARG[3] {
          { conditionObject->registerNum },
          { 1 },
          // go up by two: one to iterate passed the last instruction
          // one to increment passed the GO op
          { (int) trueInstructions->size() + 2 }}
    });

    instructions.reserve(instructions.size()
                         + distance(trueInstructions->begin(),
                                    trueInstructions->end()));
    instructions.insert(instructions.end(),
                        trueInstructions->begin(),
                        trueInstructions->end());

    if (falseBlock == NULL) {
      return;
    }

    auto falseScope = scope->createChild(false);
    auto falseInstructions = falseBlock->generate(falseScope);

    instructions.push_back(GInstruction { GOPCODE::GO, new GOPARG[1] {
          { (int) falseInstructions->size() + 1 }
    }});

    instructions.reserve(instructions.size()
                         + distance(falseInstructions->begin(),
                                    falseInstructions->end()));
    instructions.insert(instructions.end(),
                        falseInstructions->begin(),
                        falseInstructions->end());
  }

  GIndex* PConstantArray::generateExpression(GScope* scope,
                                             GInstructionVector& instructions) {
    auto sizeObject = scope->allocateObject(getInt32Type());
    instructions.push_back(GInstruction {
        LOAD_CONSTANT_INT, new GOPARG[2] { sizeObject->registerNum, (int) elements.size() }
    });

    auto arrayObject = scope->allocateObject(getNoneType());
    auto type = getNoneType();
    instructions.push_back(GInstruction {
        ARRAY_ALLOCATE, new GOPARG[2] { arrayObject->registerNum, sizeObject->registerNum }
    });
    auto indexObject = scope->allocateObject(getInt32Type());

    for (int i = 0; i < elements.size(); i++) {
      auto element = elements[i]->generateExpression(scope, instructions);

      instructions.push_back(GInstruction {
          LOAD_CONSTANT_INT, new GOPARG[2] { indexObject->registerNum, i }
      });

      instructions.push_back(GInstruction { ARRAY_SET_VALUE, new GOPARG[3] {
            arrayObject->registerNum, indexObject->registerNum, element->registerNum
      }});

      type = element->type;
    }
    arrayObject->type = getArrayType(type);
    return arrayObject;
  }

  GIndex* PArrayAccess::generateExpression(GScope* scope,
                                            GInstructionVector& instructions) {
    auto valueObject = value->generateExpression(scope, instructions);
    auto indexObject = index->generateExpression(scope, instructions);
    auto objectRegister = scope->allocateObject(valueObject->type->subTypes[0]);
    if (indexObject->type != getInt32Type()) {
      throw ParserException("index on array is not an int");
    }
    instructions.push_back(GInstruction {
        GOPCODE::ARRAY_LOAD_VALUE, new GOPARG[3] {
          valueObject->registerNum,
          indexObject->registerNum,
          objectRegister->registerNum
        }
      });
    return objectRegister;
  }

  GIndex* PMethodCall::generateExpression(GScope* scope,
                                          GInstructionVector& instructions) {
    auto object = currentValue->generateExpression(scope, instructions);
    object = enforceLocal(scope, object, instructions);
    debug("methodcall object")
    debug("  " << object->registerNum)
    debug("  " << object->indexType)
    auto funcRegister = scope->allocateObject(getFunctionType());
    auto type = object->type;
    auto methodIdx = type->environment->getObject(methodName);
    instructions.push_back(GInstruction {
        GOPCODE::INSTANCE_LOAD_ATTRIBUTE, new GOPARG[3] {
          funcRegister->registerNum, object->registerNum, methodIdx->registerNum
        }
    });

    GOPCODE instruction;
    GIndex* returnValue;
    auto argumentRegisters = new GOPARG[2 + arguments.size()];
    if (object->type == getBuiltinModuleType()) {
      instruction = GOPCODE::BUILTIN_CALL;
      returnValue = scope->allocateObject(getNoneType());
    } else {
      instruction = GOPCODE::FUNCTION_CALL;
      auto function = type->environment->getFunction(methodName);
      if (function->argumentCount != (int) arguments.size()) {
        throw ParserException("Argument count mismatch! " +
                              std::to_string(function->argumentCount) +
                              " values passed, " +
                              std::to_string((int) arguments.size()) +
                              " expected.");
      }
      for (int i = 0; i < function->argumentCount; i++) {
        GType* expectedType = function->argumentTypes[i];
        GType* actualType = arguments[i]->getType(scope);
        if (expectedType != actualType) {
          debug(expectedType->subTypes[0]->name);
          debug(actualType->subTypes[0]->name);
          throw ParserException("Argument types mismatch! "
                                "expected " + expectedType->name +
                                ", found " + actualType->name);
        }
      }

      returnValue = scope->allocateObject(function->returnType);
    }
    argumentRegisters[0].registerNum = returnValue->registerNum;
    argumentRegisters[1].registerNum = funcRegister->registerNum;
    for (int i = 0; i < (int) arguments.size(); i++) {
      auto index = arguments[i]->generateExpression(scope, instructions);
      index = enforceLocal(scope, index, instructions);
      argumentRegisters[i + 2].registerNum = index->registerNum;
    }

    instructions.push_back(GInstruction {
        instruction, argumentRegisters
    });
    return returnValue;
  }

  GIndex* PPropertyAccess::generateExpression(GScope* scope,
                                              GInstructionVector& instr) {
    debug("accessing property...")

    auto valueObject = currentValue->generateExpression(scope, instr);
    valueObject = enforceLocal(scope, valueObject, instr);

    auto objectType = valueObject->type;
    auto attribute = objectType->environment->getObject(propertyName);

    if (attribute == NULL) {
      throw ParserException("unable to retrieve type for property " + propertyName);
    }

    return new GIndex {
      .indexType = OBJECT_PROPERTY,
      .objectIndex = valueObject,
      .registerNum = attribute->registerNum,
      .type = attribute->type
    };
  }

  // generates the instructions to parse the array
  void parseArrayIterator(std::string varName, GIndex* array, PBlock* body,
                          GScope* scope, GInstructionVector& instructions) {
    GScope* forScope = scope->createChild(false);
    auto iteratorIndex = scope->allocateObject(getInt32Type());
    instructions.push_back(GInstruction {
        LOAD_CONSTANT_INT, new GOPARG[2] { iteratorIndex->registerNum, 0 }
    });

    auto iteratorObject = forScope->addObject(varName, array->type->subTypes[0]);

    auto zero = scope->allocateObject(getInt32Type());
    instructions.push_back(GInstruction {
        LOAD_CONSTANT_INT, new GOPARG[2] { zero->registerNum, 0 }
    });

    auto one = scope->allocateObject(getInt32Type());
    instructions.push_back(GInstruction {
        LOAD_CONSTANT_INT, new GOPARG[2] { one->registerNum, 1 }
    });

    auto arraySize = scope->allocateObject(getInt32Type());
    instructions.push_back(GInstruction {
        ARRAY_LOAD_LENGTH, new GOPARG[2] { array->registerNum, arraySize->registerNum }
    });

    auto conditionObject = scope->allocateObject(getBoolType());

    // initialize statement

    auto forLoopStart = instructions.size();
    instructions.push_back(GInstruction {
        ARRAY_LOAD_VALUE, new GOPARG[3] {
          array->registerNum,
          iteratorIndex->registerNum,
          iteratorObject->registerNum
        }
    });

    auto statements = body->generate(forScope);
    instructions.reserve(instructions.size()
                         + distance(statements->begin(), statements->end()));
    instructions.insert(instructions.end(), statements->begin(), statements->end());

    instructions.push_back(GInstruction {
        GOPCODE::ADD_INT, new GOPARG[3] {
          iteratorIndex->registerNum,
          one->registerNum,
          iteratorIndex->registerNum
        }
    });

    instructions.push_back(GInstruction {
        LESS_THAN_INT, new GOPARG[3] {
          iteratorIndex->registerNum, arraySize->registerNum, conditionObject->registerNum }
      });

    instructions.push_back(GInstruction {
        BRANCH, new GOPARG[3] {
          conditionObject->registerNum,
          (int) forLoopStart - (int) instructions.size(),
          { 1 }
        }
    });
  }

  void PForeachLoop::generateStatement(GScope* scope,
                                       GInstructionVector& instructions) {
    auto iterableValue = iterableExpression->generateExpression(scope, instructions);
    // if the value is an array, we iterate through the array first
    if (iterableValue->type->name.compare("Array") == 0) {
      parseArrayIterator(variableName, iterableValue, block,
                         scope, instructions);
    } else {
      throw ParserException("unable to use foreach expression on type: " + iterableValue->type->name);
    }
  }
}
