#include "nodes.hpp"
#include "exceptions.hpp"
#include <iostream>

// #define debug(s);
#define debug(s) std::cout << s << std::endl;

using namespace VM;
using namespace lexer;
using namespace codegen;

namespace parser {

  GType* evaluateType(std::string typeName) {
    if (typeName == "Int") { return getInt32Type(); }
    else if (typeName == "Bool") { return getBoolType(); }
    else if (typeName == "String") { return getStringType(); }
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
    auto instructions = new GInstructionVector;
    for (auto statement : statements) {
      statement->generateStatement(scope, *instructions);
    }
    scope->finalize();
    return instructions;
  }

  GIndex* PCall::generateExpression(GScope* scope,
                                     GInstructionVector& instructions) {
    if (name == "print") {
      auto argument = arguments[0]->generateExpression(scope, instructions);
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
        throw ParserException("Unable to print class " + type->name);
      }

      instructions.push_back(GInstruction {
          op, new GOPARG[1] { { argument->registerNum } }
      });

    } else if (auto functionIndex = scope->getObject(name)) {

      if (functionIndex->type != getFunctionType()) {
        throw ParserException("" + name + " is not a function!");
      }

      auto function = scope->getFunction(name);

      auto opArgs = new std::vector<GOPARG>;
      // first OPARG is the function pointer
      opArgs->push_back(GOPARG{ .registerNum = functionIndex->registerNum });

      // second OPARG is the return object
      auto returnObject = scope->allocateObject(function->returnType);
      opArgs->push_back(GOPARG{ returnObject->registerNum });

      // third on are the actual arguments
      for (auto argument : arguments) {
        auto object = argument->generateExpression(scope, instructions);
        opArgs->push_back(GOPARG { object->registerNum });
      }

      instructions.push_back(GInstruction { GOPCODE::FUNCTION_CALL, &(*opArgs)[0] });
      return returnObject;
    } else {
      throw ParserException("Unable to call method " + name);
    }

    return NULL;
  }

  void PDeclare::generateStatement(GScope* scope,
                                   GInstructionVector& instructions) {
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

      instructions.push_back(GInstruction {
          SET, new GOPARG[2] { {value->registerNum}, {ident->registerNum} }
      });
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
    auto lhsObject = lhs->generateExpression(scope, instructions);
    auto rhsObject = rhs->generateExpression(scope, instructions);

    // cast as necessary
    /* if (lhsObject->type == getFloatType() && rhsObject->type == getInt32Type()) {
      rhsObject = intToFloat(rhsObject, instructions);
    }
    if (lhsObject->type == getInt32Type() && rhsObject->type == getFloatType()) {
      lhsObject = intToFloat(lhsObject, instructions);
      } */

    if (lhsObject->type != rhsObject->type) {
      throw ParserException("type mismatch during binary operation!");
    }

    GIndex* resultObject;
    VM::GOPCODE opCode;

    switch (op) {
    case L::LESS_THAN:
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
    auto object = scope->getObject(name);
    if (object->isGlobal) {
      auto newObject = scope->allocateObject(object->type);
      instructions.push_back({
          GOPCODE::GLOBAL_LOAD,
          new VM::GOPARG[2] { {newObject->registerNum}, {object->registerNum} }
      });
      return newObject;
    }

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

    if (scope->getObject(name) != NULL) {
      throw ParserException("Cannot redeclare " + name);
    }

    auto argumentNames = new std::string[arguments.size()];
    auto argumentTypes = new GType*[arguments.size()];

    GScope* functionScope = scope->createChild(true, false);

    int i = 0;
    for (auto argument : arguments) {
      auto type = evaluateType(argument->second);

      functionScope->addObject(argument->first, type);
      argumentNames[i] = argument->first;
      argumentTypes[i] = type;
      i++;
    }

    auto index = scope->addFunction(name, new GFunction {
        .argumentCount = (int) arguments.size(),
        .argumentNames = argumentNames,
        .argumentTypes = argumentTypes,
        .environment = *(functionScope->environment),
        .instructions = NULL,
        .returnType = evaluateType(returnType),
          }, this, functionScope);
    auto functionIndex = scope->environment->functionTable[index->registerNum];

    instructions.push_back(GInstruction {
        GOPCODE::FUNCTION_CREATE, new VM::GOPARG[2] {
          { index->registerNum }, { functionIndex }
        }});
  }

  void PFunctionDeclaration::generateBody(GFunction* function, GScope* functionScope) {
    auto vmBody = body->generate(functionScope);
    vmBody->push_back(GInstruction { END, 0 });
    function->instructions = &(*vmBody)[0];
  }

  void PClassDeclaration::generateStatement(GScope*, GInstructionVector&) {
    int attributeSize = attributes.size();
    auto attributeTypes = new GType*[attributeSize];
    auto attributeNames = new std::string[attributeSize];

    int i = 0;
    for (auto& kv: attributes) {
      attributeNames[i] = kv.first;
      attributeTypes[i] = evaluateType(kv.second);
    }

    auto type = new GType{
      .name = name,
      .subTypes = attributeTypes,
      .attributeNames = attributeNames,
      .subTypeCount = attributeSize
    };
  }

  void PIfElse::generateStatement(GScope* scope,
                                  GInstructionVector& instructions) {
    auto conditionObject = condition->generateExpression(scope, instructions);

    auto trueScope = scope->createChild(false, true);
    auto trueInstructions = trueBlock->generate(trueScope);

    auto falseScope = scope->createChild(false, true);
    auto falseInstructions = falseBlock->generate(falseScope);

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

  GIndex* PArray::generateExpression(GScope* scope,
                                      GInstructionVector& instructions) {
    auto arrayObject = scope->allocateObject(getNoneType());
    auto type = getNoneType();
    instructions.push_back(GInstruction {
        ARRAY_ALLOCATE, new GOPARG[2] { arrayObject->registerNum, (int) elements.size() }
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
    if (indexObject->type->classifier != INT32) {
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

  // generates the instructions to parse the array
  void parseArrayIterator(std::string varName, GIndex* array, PBlock* body,
                          GScope* scope, GInstructionVector& instructions) {
    GScope* forScope = scope->createChild(false, true);
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
    if (iterableValue->type->classifier == ARRAY) {
      parseArrayIterator(variableName, iterableValue, block,
                         scope, instructions);
    } else {
      throw ParserException("unable to use foreach expression on type: " + iterableValue->type->name);
    }
  }
}
