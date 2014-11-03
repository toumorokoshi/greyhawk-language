#include "nodes.hpp"
#include "exceptions.hpp"
#include <iostream>

// #define debug(s);
#define debug(s) std::cout << s << std::endl;

using namespace VM;
using namespace lexer;

namespace parser {

  GType* evaluateType(std::string typeName) {
    if (typeName == "Int") { return getInt32Type(); }
    else if (typeName == "Bool") { return getBoolType(); }
    else if (typeName == "String") { return getStringType(); }
    else if (typeName == "None") { return getNoneType(); }

    throw ParserException("Cannot find class " + typeName);
  }

  GInstruction* generateRoot(VM::GScope* scope, PBlock* block) {
    auto instructions = block->generate(scope);

    instructions->push_back(GInstruction { END, NULL });
    return &(*instructions)[0];
  }

  GInstructionVector* PBlock::generate(VM::GScope* scope) {
    auto instructions = new GInstructionVector;
    for (auto statement : statements) {
      statement->generateStatement(scope, *instructions);
    }
    return instructions;
  }

  GObject* PCall::generateExpression(VM::GScope* scope,
                                     GInstructionVector& instructions) {
    if (name == "print") {
      auto argument = arguments[0]->generateExpression(scope, instructions);
      GOPCODE op;
      switch (argument->type->classifier) {
      case BASICTYPES::STRING:
        op = GOPCODE::PRINT_STRING;
        break;
      case BASICTYPES::INT32:
        op = GOPCODE::PRINT_INT;
        break;
      }
      instructions.push_back(GInstruction {
          op, new GOPARG[1] { { argument->registerNum } }
        });
      return getNoneObject();

    } else if (auto function = scope->getFunction(name)) {
      auto opArgs = new std::vector<GOPARG>;
      // first OPARG is the function pointer
      opArgs->push_back(GOPARG{ .function = function });

      // second OPARG is the return object
      auto returnObject = scope->frame->allocateObject(function->returnType);
      opArgs->push_back(GOPARG{ returnObject->registerNum });

      // third on are the actual arguments
      for (auto argument : arguments) {
        auto object = argument->generateExpression(scope, instructions);
        opArgs->push_back(GOPARG { object->registerNum });
      }

      instructions.push_back(GInstruction { GOPCODE::CALL, &(*opArgs)[0] });
      return returnObject;
    }

    return getNoneObject();
  }

  void PDeclare::generateStatement(VM::GScope* scope,
                                   GInstructionVector& instructions) {
    auto value = expression->generateExpression(scope, instructions);
    auto newVar = scope->addObject(name, value->type);
    instructions.push_back(GInstruction {
        SET, new GOPARG[2] { value->registerNum, newVar->registerNum }
      });
  }

  /* GObject* intToFloat(GObject* integer, GInstructionVector& instructions) {
    auto castResult = new GObject { getFloatType(), {0}};
    instructions.push_back(GInstruction {
        GOPCODE::INT_TO_FLOAT, new GObject*[2] { integer, castResult }
      });
    return castResult;
    } */

  GObject* PBinaryOperation::generateExpression(VM::GScope* scope,
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

    GObject* resultObject;
    VM::GOPCODE opCode;

    switch (op) {
    case L::LESS_THAN:
      resultObject = scope->frame->allocateObject(getBoolType());
      opCode = GOPCODE::LESS_THAN_INT;
      break;

    case L::PLUS: {
      if (lhsObject->type == getFloatType()) {
        resultObject = new GObject { getFloatType(), { 0 }};
        opCode = GOPCODE::ADD_FLOAT;
        break;

      } else if (lhsObject->type == getInt32Type()) {
        resultObject = scope->frame->allocateObject(getInt32Type());
        opCode = GOPCODE::ADD_INT;
        break;
      }
    }

    default:
      throw ParserException("binary op not implemented");
    }

    instructions.push_back(GInstruction { opCode, new GOPARG[3] {
          lhsObject->registerNum, rhsObject->registerNum, resultObject->registerNum }
      });
    return resultObject;
  }

  void PIncrement::generateStatement(VM::GScope* scope,
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

  GObject* PConstantString::generateExpression(VM::GScope* s, GInstructionVector& i) {
    auto target = s->frame->allocateObject(getStringType());
    i.push_back(GInstruction {
        GOPCODE::LOAD_CONSTANT_STRING, new VM::GOPARG[2] {
          { target->registerNum }, GOPARG { .asString = value.c_str() }
        }});
    return target;
  }

  GObject* PIdentifier::generateExpression(VM::GScope* scope, GInstructionVector&) {
    auto object = scope->getObject(name);

    if (object == NULL) {
      throw ParserException("Object " + name + " is not defined in this scope!");
    }
    return object;
  }

  void PForLoop::generateStatement(VM::GScope* scope,
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

    auto frame = new GFrame();
    GScope functionScope(scope, frame);
    for (auto argument : arguments) {
      functionScope.addObject(argument->first, evaluateType(argument->second));
    }

    auto vmBody = body->generate(&functionScope);
    vmBody->push_back(GInstruction { END, { 0 }});

    auto function = new GFunction {
      .returnType = evaluateType(returnType),
      .instructions = &(*vmBody)[0],
      .registerCount = frame->registerCount,
      .argumentCount = (int) arguments.size()
    };

    scope->addFunction(name, function);
  }

  void PIfElse::generateStatement(VM::GScope* scope,
                                  GInstructionVector& instructions) {
    auto conditionObject = condition->generateExpression(scope, instructions);

    GScope trueScope(scope);
    auto trueInstructions = trueBlock->generate(&trueScope);

    GScope falseScope(scope);
    auto falseInstructions = falseBlock->generate(&falseScope);

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

  /* GObject* PArray::generateExpression(VM::GScope* scope,
                                      GInstructionVector& instructions) {
    auto array = new GObject*[elements.size()];
    auto type = getNoneType();
    for (int i = 0; i < elements.size(); i++) {
      array[i] = elements[i]->generateExpression(scope, instructions);
      type = array[i]->type;
    }
    auto arrayObject = new GObject { getArrayType(type), { 0 } };
    arrayObject->value.asArray = new GArray {
      array, (int) elements.size()
    };
    return arrayObject;
    } */

  /* GObject* PArrayAccess::generateExpression(VM::GScope* scope,
                                            GInstructionVector& instructions) {
    auto valueObject = value->generateExpression(scope, instructions);
    auto indexObject = index->generateExpression(scope, instructions);
    auto objectRegister = new GObject { valueObject->type->subTypes, {0} };
    if (indexObject->type->classifier != INT32) {
      throw ParserException("index on array is not an int");
    }
    instructions.push_back(GInstruction {
        GOPCODE::ACCESS_ELEMENT, new GObject*[3] {
          valueObject, indexObject, objectRegister
        }
      });
    return objectRegister;
    } */

  // generates the instructions to parse the array
  /* void parseArrayIterator(std::string varName, GObject* array, PBlock* body,
                          GScope* scope, GInstructionVector& instructions) {
    GScope forScope(scope);
    auto iteratorIndex = new GObject { getInt32Type(), {0}};
    auto iteratorObject = new GObject { array->type->subTypes, {0}};
    auto zero = new GObject { getInt32Type(), { 0 }};
    auto one = new GObject { getInt32Type(), { 1 }};
    auto arraySize = new GObject { getInt32Type(), { 0 }};
    // initialize statement
    forScope.locals[varName] = iteratorObject;
    instructions.push_back(GInstruction { GOPCODE::LENGTH, new GObject*[2] { array, arraySize }});
    instructions.push_back(GInstruction { GOPCODE::SET, new GObject*[2] { zero, iteratorIndex }});

    auto forLoopStart = instructions.size();
    instructions.push_back(GInstruction { GOPCODE::ACCESS_ELEMENT, new GObject*[3] {
          array, iteratorIndex, iteratorObject
        }});
    auto statements = body->generate(&forScope);
    instructions.reserve(instructions.size()
                         + distance(statements->begin(), statements->end()));
    instructions.insert(instructions.end(), statements->begin(), statements->end());
    auto conditionObject = new GObject { getBoolType(), { 0 }};
    instructions.push_back(GInstruction {
        GOPCODE::ADD_INT, new GObject*[3] { iteratorIndex, one, iteratorIndex }
      });
    instructions.push_back(GInstruction {
        GOPCODE::LESS_THAN, new GObject*[3] { iteratorIndex, arraySize, conditionObject }
      });
    instructions.push_back(GInstruction {
        GOPCODE::BRANCH, new GObject*[3] {
          conditionObject,
            new GObject { getInt32Type(), { (int) forLoopStart - (int) instructions.size() }},
            new GObject { getInt32Type(), { 1 }}
        }
      });
      } */

  /* void PForeachLoop::generateStatement(VM::GScope* scope,
                                       GInstructionVector& instructions) {
    auto iterableValue = iterableExpression->generateExpression(scope, instructions);
    // if the value is an array, we iterate through the array first
    if (iterableValue->type->classifier == ARRAY) {
      parseArrayIterator(variableName, iterableValue, block,
                         scope, instructions);
    } else {
      throw ParserException("foreach loop not yet implemented!");
    }
    } */
}
