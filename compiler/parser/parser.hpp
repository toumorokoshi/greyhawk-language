#include "../lexer/tokens.hpp"
#include "./node.hpp"
#include "./exceptions.hpp"
#include <functional>

#ifndef PARSER_HPP
#define PARSER_HPP

namespace parser {

  NBlock* parseBlock(lexer::TokenVector::iterator& token_position,
                         lexer::TokenVector& tokens);

  NStatement* parseStatement(lexer::TokenVector::iterator& token_position,
                             lexer::TokenVector& tokens);

  NExpression* parseExpression(lexer::TokenVector::iterator& token_position,
                               lexer::TokenVector& tokens);

  NMethodCall* parseMethodCall(lexer::TokenVector::iterator& token_position,
                               lexer::TokenVector& tokens);

  NExpression* parseNumeric(lexer::TokenVector::iterator& token_position,
                            lexer::TokenVector& tokens);

  NExpression* parseSingleNumeric(lexer::TokenVector::iterator& token_position,
                                  lexer::TokenVector& tokens);
}

#endif
