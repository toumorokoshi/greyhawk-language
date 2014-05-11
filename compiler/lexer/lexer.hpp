#include <string>
#include <exception>
#include <boost/regex.hpp>

#ifndef LEXER_HPP

#define LEXER_HPP

namespace lexer {

    // extend exceptions from here if they have to do with
    // the lexer specifically
    class LexerException: public std::exception {
    public:
      const std::string message;
      LexerException(std::string _message) : message(_message) {}
    };

    class Token {
    public:
      virtual std::string getDescription() { return "nothing"; }
    };


    class Keyword : public Token {
    public:
      const std::string symbol;
      Keyword(std::string _symbol) : symbol(_symbol) {}
      std::string getDescription() { return symbol; }
    };


    typedef std::vector<Token> TokenVector;
    typedef std::vector<Keyword> KeywordVector;

    TokenVector tokenize(std::string input);

    // utility methods
    bool isAlpha(char);
    bool isNumeric(char);
    bool isAlphaNumeric(char);

}

#endif
