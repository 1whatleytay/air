#pragma once

#include <utils/parser.h>

//#include <stack>
#include <string>
#include <vector>

class Parser {
    int rollbackIndex = 0;
    int index = 0;
    std::string text;

    int selectIndex = 0;

    bool end(int offset = 0);
    void pushIndex();

public:
    enum Mode {
        Generic,
        Original,
    };

    Mode mode = Generic;

    bool reachedEnd(int offset = 0);

    void seekNext(const std::string &word);
    void seekNextWords(const std::vector<std::string> &word);
    void rollback();

    void select();
    std::string clip();
    void back();

    std::string untilNextSymbolClosing(char opening, char symbol, int depth = 1);
//    std::string untilNextSymbol(char symbol);
    std::string untilNextSymbols(const std::vector<char> &tests);
    std::string untilNextWord(const std::string &word);
    std::string untilNextWords(const std::vector<std::string> &tests);
    std::string untilNextWhitespace();

//    std::string toString();

//    char lastSymbol();
    std::string lastWord();

//    char nextSymbol();
    std::string nextWord();
    std::string peekWord();

    static std::string trim(std::string text);

    explicit Parser(std::string nText);
};
