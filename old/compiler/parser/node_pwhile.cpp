#include "nodes.hpp"
#include "parser.hpp"

using namespace lexer;
using namespace VM;

namespace parser {

  YAML::Node* PWhile::toYaml() {
    auto node = new YAML::Node();
    (*node)["while"]["condition"] = *condition->toYaml();
    (*node)["while"]["body"] = *body->toYaml();
    return node;
  }

  void PWhile::generateStatement(codegen::GScope* scope,
                                 GInstructionVector& instr) {
    int loopStart = instr.size();
    auto whileScope = scope->createChild(false);
    auto statements = body->generate(whileScope);

    auto conditionObject = condition->generateExpression(scope, instr);
    if (conditionObject->type != getBoolType()) {
      throw ParserException("While loop condition is not a boolean! found "
                            + conditionObject->type->name + " instead.");
    }

    instr.push_back(GInstruction {
        GOPCODE::BRANCH, new GOPARG[3] {
          { conditionObject->registerNum }, {1}, { (int) statements->size() + 2}
        }
    });

    instr.reserve(instr.size() + distance(statements->begin(), statements->end()));
    instr.insert(instr.end(), statements->begin(), statements->end());
    instr.push_back(GInstruction { GOPCODE::GO, new GOPARG[1] { - ((int) instr.size() - loopStart) }});
  }

  PWhile* Parser::parseWhile() {
    _validateToken(WHILE, "expected a while for a while loop");
    token_position++;

    auto expression = parseExpression();

    _validateToken(COLON, "expected a : for a while loop");
    token_position++;

    _validateToken(INDENT, "expected an indent for a for loop");
    token_position++;

    auto block = parseBlock();

    _validateToken(UNINDENT, "expected an unindent for a for loop");
    token_position++;

    return new PWhile(expression, block);
  }
}
