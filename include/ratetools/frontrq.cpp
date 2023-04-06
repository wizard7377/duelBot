#include "ratesys.hpp"

namespace rQ {

frontRQ::frontRQ(std::function<void(snowPair,snowPair)> inFunc) {
    this->sTempFunc = inFunc;
}
}