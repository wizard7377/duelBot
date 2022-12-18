#include <string>
#include <iostream>
#include "gameLogic.hpp"

namespace game {



baseGameLogic::baseGameLogic() {
}

bool baseGameLogic::makeMove(int inputOne, int inputTwo) { return false; }
std::string baseGameLogic::convertIntToString(int logicMove) {
	return (this->moveNames[logicMove]);
}
int baseGameLogic::convertStringToInt(std::string userMove) {

	int i = 0;
	for (auto x : *(this->moveNames)) {
		//if (x == userMove) { return i; };
		//Not working as of last check (check again)
		
		std::cout << x << std::endl;
		i++;

	}
	return i;
}

}
