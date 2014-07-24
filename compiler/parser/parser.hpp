#include "../lexer/tokens.hpp"
#include "../node.hpp"
#include "./exceptions.hpp"
#include <functional>

#ifndef PARSER_HPP
#define PARSER_HPP

namespace parser {

  NStatement* parseStatement(lexer::TokenVector::iterator& token_position,
                            lexer::TokenVector::iterator token_end);

  NExpression* parseExpression(lexer::TokenVector::iterator& token_position,
                               lexer::TokenVector::iterator token_end);

  NMethodCall* parseMethodCall(lexer::TokenVector::iterator& token_position,
                               lexer::TokenVector::iterator token_end);

  NExpression* parseNumeric(lexer::TokenVector::iterator& token_position,
                            lexer::TokenVector::iterator token_end);

  NExpression* parseSingleNumeric(lexer::TokenVector::iterator& token_position,
                                  lexer::TokenVector::iterator token_end);
}

#endif
