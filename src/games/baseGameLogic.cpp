#include <string>
#include <iostream>
#include <vector>
#include <cmath>

#include "gameLogic.hpp"
#define BIG_VAL 1000000
template <typename T>
T getValAt(int index, std::vector<std::vector<T>> * inVec) {
	return (*inVec)[(int)(std::floor(index/(*inVec).size()))][(int)(index % (*inVec)[0].size())];
}
namespace game {



baseGameLogic::baseGameLogic() {
}

std::string baseGameLogic::convertIntToString(int logicMove) {
	return (getValAt<std::string>(logicMove,(&this->moveNamesCon)));
}

int baseGameLogic::convertStringToInt(std::string userMove) {
	//TODO Make this less worse
	if (this->isOneD()) {
		int i = 0;
		for (i = 0; i < this->moveNames.size(); i++) {
			if (this->moveNames[i][0] == userMove) { return i; };
		}
		return i;
	} else {
		int i = 0;
		for (i = 0; i < this->moveNames.size(); i++) {
			if (this->convertIntToString(i) == userMove) { return i; };
		}
		return i;
	}
}
int baseGameLogic::getAt(int x, int y) {
	if ((x < 0) or (y < 0)) return BIG_VAL; 
	if (this->boardItems->size() > x) {
		if (this->boardItems->at(x).size() > y) return (*(this->boardItems))[x][y];
		else return BIG_VAL;
	}
	else return BIG_VAL; 
}
}
