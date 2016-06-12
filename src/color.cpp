#include <stdexcept>
#include "color.h"

Color otherColor(Color color) {
    if (color == NONE) {
        throw std::runtime_error("NONE does not have an opposite");
    } else {
        return color == BLACK ? WHITE : BLACK;
    }
}
