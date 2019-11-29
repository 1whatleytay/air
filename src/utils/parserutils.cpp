#include <utils/parserutils.h>

#include <utils/parser.h>

#include <sstream>

std::string parseString(Parser &parser, const std::string &quote) {
    std::string self = parser.nextWord();
    assert(self == quote);

    std::stringstream output;
    parser.mode = Parser::Mode::Original;

    while (!parser.reachedEnd()) {
        output << parser.untilNextWords({"\\", quote});
        std::string next = parser.nextWord();
        if (next == quote) {
            break;
        } else if (next == "\\") {
            std::string nextSymbol = parser.nextWord();
            throw std::exception();
        }
    }

    parser.mode = Parser::Mode::Generic;

    return output.str();
}
