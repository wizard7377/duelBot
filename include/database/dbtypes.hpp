#pragma once

#include <variant>
#include <vector>
#include <string>

using namespace std;

using cResType = variant<int,uint64_t,bool,string>;
using reSet = vector<cResType>;