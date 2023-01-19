#include <vector>
#include <string>
#include "gameLogic.hpp"



enum checks {
	EMPTY,
	WHITEPAWN,
	WHITEKING,
	BLACKPAWN,
	BLACKKING,
};
const std::string checkRowNames[] = {"a","b","c","d","e","f","g","h"};
const std::string checkColNames[] = {"1","2","3","4","5","6","7","8"};

namespace game {
	checkersLogic::checkersLogic() {
		this->boardItems = new std::vector<std::vector<int>>{
			{EMPTY,WHITEPAWN,EMPTY,WHITEPAWN,EMPTY,WHITEPAWN,EMPTY,WHITEPAWN},
			{WHITEPAWN,EMPTY,WHITEPAWN,EMPTY,WHITEPAWN,EMPTY,WHITEPAWN,EMPTY},
			{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
			{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
			{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
			{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
			{EMPTY,BLACKPAWN,EMPTY,BLACKPAWN,EMPTY,BLACKPAWN,EMPTY,BLACKPAWN},
			{BLACKPAWN,EMPTY,BLACKPAWN,EMPTY,BLACKPAWN,EMPTY,BLACKPAWN,EMPTY}
		};

		this->moveNames = std::vector<std::vector<std::string>>();
		for (int i = 0; i < 8; i++) {
			this->moveNames.push_back({});
			for (int ii = 0; ii < 8; ii++) {
				this->moveNames[i].push_back(checkRowNames[i]+checkColNames[ii]);
			
			}
		}


	
	}
	bool checkersLogic::makeMove(int inputOne,int inputTwo) {
		return true;
	}
	bool checkersLogic::getWinner() { return true; }














}
