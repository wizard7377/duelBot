#pragma once

#include <string>

#define BLACK_TERM 30
#define RED_TERM 31
#define GREEN_TERM 32
#define YELLOW_TERM 33
#define BLUE_TERM 34
#define PURPLE_TERM 35
#define CYAN_TERM 36
#define WHITE_TERM 37
#define DEF_TERM 39

static std::string colorForm(std::string inStr, int cId) {
    return ("\033[" + std::to_string(cId) + "m" + inStr + "\033[0m");
}