
#pragma once 

#include <string>

const std::string rowNames[] = {"a","b","c","d","e","f","g","h"};
const std::string colNames[] = {"1","2","3","4","5","6","7","8"};

std::string* getAllBoardNames();

namespace game {
class baseGameLogic {
	public:
		baseGameLogic();
		std::string gameId;
		int* boardItems;
		int* allowedMoves;
		virtual bool makeMove(int inputOne, int inputTwo = 0);
	protected:

		std::string* moveNames;
		bool userTurn = true;
		std::string convertIntToString(int logicMove);
		int convertStringToInt(std::string userMove);

};

class chessLogic : public baseGameLogic {
	public:
		chessLogic();
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

};

class ticTacToeLogic : public baseGameLogic {
	public:
		ticTacToeLogic();
		bool makeMove(int inputOne, int inputTwo = 0) override;
		int boardItems[3][3] = {
			{0,0,0},
			{0,0,0},
			{0,0,0}
		};
	private:
		bool checkForEnd();
		



		





};

}


