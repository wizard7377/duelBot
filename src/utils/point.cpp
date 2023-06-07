#include "utl.hpp"
#include <cmath>

using namespace dpp;

namespace utl {
point::point(int inX, int inY, int inSize) { 
    this->x = inX;
    this->y = inY;
    this->size = inSize;
}
point::point(int inIndex,int inSize, bool xIsOne) {
    this->size = inSize;
    if (xIsOne) {
        this->y = (int)(std::floor(inIndex/inSize));
        this->x = (int)(inIndex % inSize);
    } else {
        this->x = (int)(std::floor(inIndex/inSize));
        this->y = (int)(inIndex % inSize);
    }
}
int point::getVal(bool xIsOne) {
    if (xIsOne) {
        return ((this->size * this->y) + this->x);
    } else {
         return ((this->size * this->x) + this->y);
    }
}
std::string point::tString() {
    return ("("+std::to_string(this->x)+", " + std::to_string(this->y) + ")");
}
}
