
#pragma once 

#include <string>
#include <vector>

const std::string rowNames[] = {"a","b","c","d","e","f","g","h"};
const std::string colNames[] = {"1","2","3","4","5","6","7","8"};

std::string* getAllBoardNames();

namespace game {
class baseGameLogic {
	public:
		baseGameLogic();
		std::string gameId;
		std::vector<std::vector<int>> boardItems;
		int* allowedMoves;
		virtual bool getWinner() { return false; }
		virtual bool makeMove(int inputOne, int inputTwo) { return false; }
		virtual bool makeMove(int inputOne) { return false; }
		std::string convertIntToString(int logicMove);
		int convertStringToInt(std::string userMove);
	protected:

		std::string* moveNames;
		bool userTurn = true;

};

class chessLogic : public baseGameLogic {
	public:
		chessLogic();
		std::vector<std::vector<int>> boardItems = {
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
		bool makeMove(int inputOne) override;
		bool makeMove(int inputOne,int inputTwo) override { return false ; };
		

		bool getWinner() override;
		std::vector<std::vector<int>> boardItems = {
			{0,0,0},
			{0,0,0},
			{0,0,0}
		};
	private:
		bool checkForEnd();
		



		





};

}


