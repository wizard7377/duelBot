#include <vector>
#include "gameLogic.hpp"



enum checks {
	EMPTY,
	WHITEPAWN,
	WHITEKING,
	BLACKPAWN,
	BLACKKING,
};

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
	
	}
	bool checkersLogic::makeMove(int inputOne,int inputTwo) {
		return true;
	}
	bool checkersLogic::getWinner() { return true; }














}
