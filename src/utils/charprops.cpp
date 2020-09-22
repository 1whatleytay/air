#include <utils/charprops.h>

bool isDigits(const std::string &text) {
    for (char a : text) {
        if (!std::isdigit(a))
            return false;
    }

    return true;
}
