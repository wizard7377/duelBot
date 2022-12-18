#include <string>

#include <iostream>
#include "gameLogic.hpp"
//empty pawn knight bishop rook queen king


const std::string rowNames[] = {"a","b","c","d","e","f","g","h"};
const std::string colNames[] = {"1","2","3","4","5","6","7","8"};


std::string* getAllBoardNames() {
	static std::string returnArray[64];

	for (int i = 0; i < 8; i++) {
		for (int ii = 0; ii < 8; ii++) {
			returnArray[(i*8)+ii] = (rowNames[i]+colNames[ii]);
		}
	}
	return returnArray;
}




namespace game {



baseGameLogic::baseGameLogic() {
}
//bool baseGameLogic::makeMove(int inputOne);

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


chessLogic::chessLogic() {
	this->moveNames = getAllBoardNames();
	this->convertStringToInt("nothing");
	this->gameId = "0001";
}
//bool chessLogic::makeMove(int inputOne, int inputTwo);







	





}
