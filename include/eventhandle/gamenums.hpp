#pragma once

#include <unordered_map>
#include <string>

using namespace std;

unordered_map<string,int> gameNums = {
    {"tictactoe",0},
    {"checkers",1}
};
//this
//global
//both
//all
//allbut
unordered_map<string,int> scopeNums = {
    {"this",0},
    {"global",1},
    {"both",2},
    {"all",3},
    {"allbut",4}
};