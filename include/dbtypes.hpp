#pragma once

#include <variant>
#include <vector>
#include <string>
#include <cstdint>
class erVal {};
using namespace std;

using cResType = variant<int,uint64_t,bool,string,erVal,nullptr_t>;
using reSet = vector<cResType>;