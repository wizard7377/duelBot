#include <string>

#include <iostream>
#include "gameLogic.hpp"
//empty pawn knight bishop rook queen king



std::string* getAllBoardNames() {
	static std::string returnArray[64];

	for (int i = 0; i < 8; i++) {
		for (int ii = 0; ii < 8; ii++) {
			returnArray[(i*8)+ii] = (rowNames[i]+colNames[ii]);
		}
	}
	return returnArray;
}

