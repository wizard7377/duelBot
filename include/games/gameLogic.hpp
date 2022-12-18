
#pragma once 

#include <string>

namespace game {
class baseGameLogic {
	public:
		std::string gameId;
		int* boardItems;
		int* allowedMoves;
		baseGameLogic();
		bool makeMove(int inputOne);
	protected:

		std::string* moveNames;
		std::string convertIntToString(int logicMove);
		int convertStringToInt(std::string userMove);

};

class chessLogic : public baseGameLogic {
	public:
		int boardItems[8][8] = {
			{4,2,3,5,6,3,2,4},
			{1,1,1,1,1,1,1,1},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{7,7,7,7,7,7,7,7},
			{10,8,9,11,12,9,8,10}
		};
		chessLogic();
		bool makeMove(int inputOne, int inputTwo);

};



		





}


