
#pragma once 

#include <string>
#include <vector>
#include <map>
#include "utl.hpp"

const std::string rowNames[] = {"a","b","c","d","e","f","g","h"};
const std::string colNames[] = {"1","2","3","4","5","6","7","8"};

std::string* getAllBoardNames();

namespace game {
class baseGameLogic {
	public:
		baseGameLogic();
		bool isCapture = false;
		virtual bool isDuoMove() { return true; };
		std::string gameId;
		std::vector<std::vector<int>> * boardItems;
		bool* allowedMoves;
		virtual bool getWinner() { return false; }
		virtual bool makeMoveOld(int inputOne, int inputTwo) { return false; }
		virtual bool makeMoveOld(int inputOne) { return false; }
		virtual bool makeMove(int inputOne,int inputTwo, bool playerTurn) { return false; }
		virtual bool makeMove(int inputOne,bool playerTurn) { return false; }
		std::string convertIntToString(int logicMove);
		int convertStringToInt(std::string userMove);
		std::vector<std::vector<std::string>> moveNames;
		std::vector<std::vector<std::string>> moveNamesCon;
		std::vector<std::vector<std::string>> extraMoveNames;
		std::map<std::string,std::vector<utl::point>> capMoves;
		std::vector<std::vector<utl::point>> capMovesVec;
		std::vector<std::string> capMovesNames;
		
	protected:

		virtual void changeMoves(bool playerTurn = false) {};
		bool userTurn = true;

};

class chessLogic : public baseGameLogic {
	public:
		chessLogic();
		//std::vector<std::vector<int>> * boardItems;
		/* 
		= {
			{4,2,3,5,6,3,2,4},
			{1,1,1,1,1,1,1,1},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{7,7,7,7,7,7,7,7},
			{10,8,9,11,12,9,8,10}
		};
		*/
	private:
		void changeMoves(bool playerTurn) override {};
};

class ticTacToeLogic : public baseGameLogic {
	public:
		ticTacToeLogic();
		bool isDuoMove() override { return false; }
		bool makeMove(int inputOne,bool playerTurn) override;
		bool makeMove(int inputOne,int inputTwo, bool playerTurn) override { return false ; };
		

		bool getWinner() override;
		//std::vector<std::vector<int>> * boardItems;
		/*
		 = {
    		{0, 0, 0},
    		{0, 0, 0},
    		{0, 0, 0}
		};
		*/
	private:
		void changeMoves(bool playerTurn) override;
		bool checkForEnd();
		
};

class checkersLogic : public baseGameLogic {
	public:
		checkersLogic();
		bool makeMove(int inputOne,bool playerTurn) override { return false; };
		bool makeMove(int inputOne,int inputTwo, bool playerTurn) override;
		

		bool getWinner() override;
		//std::vector<std::vector<int>> boardItems;
		/*
		 = {
    		{0, 0, 0},
    		{0, 0, 0},
    		{0, 0, 0}
		};
		*/
	private:
		void changeMoves(bool playerTurn) override;
		bool checkForEnd();
		int pAt(int x, int y);
		
		
};

}


