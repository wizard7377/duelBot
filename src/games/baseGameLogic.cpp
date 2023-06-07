#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include "gameLogic.hpp"

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
	int i = 0;
	for (i = 0; i < this->moveNames.size(); i++) {
		if (this->convertIntToString(i) == userMove) { return i; };
	}
	return i;
}

}
