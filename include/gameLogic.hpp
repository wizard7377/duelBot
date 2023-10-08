
#pragma once 

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <concepts>
#include "utl.hpp"
//#include "gameInteraction.hpp"

/* So this is
 * A pair of a vector of moves and corresponding int positions and a int pos
*/
using posVec = std::vector<int>;
using moveInfo = std::pair<std::map<std::string,std::vector<posVec>>,posVec>;

const std::string rowNames[] = {"a","b","c","d","e","f","g","h"};
const std::string colNames[] = {"1","2","3","4","5","6","7","8"};

std::string* getAllBoardNames();

namespace game {
class baseGameLogic {
	public:
		baseGameLogic();
		//baseGameLogic(gameInt::baseGameInt& parent);	
		/*! Signals that moves involve more than two possible inputs */
		bool isCapture = false; 
		virtual bool isDuoMove() { return true; };
		virtual bool isOneD() { return false; }
		std::string gameId;
		std::vector<std::vector<int>> * boardItems; //!< Board state
		bool* allowedMoves;
		/*! Returns the player who won
		 * @brief Gets winner
		 * @return Player 1 is false, 2 is true
		 */
		virtual bool getWinner() { return false; } 
		virtual bool makeMoveOld(int inputOne, int inputTwo) { return false; } //!< @deprecated
		virtual bool makeMoveOld(int inputOne) { return false; } //!< @deprecated
		
		
		std::string convertIntToString(int logicMove);
		int convertStringToInt(std::string userMove);
		
		virtual int gameInt() { return -1; };
		
	protected:
		int getAt(int x, int y);
		virtual void changeMoves(bool playerTurn = false) {}; //!< Updates the list of moves
		bool userTurn = true;

};

class oneMove : public baseGameLogic {
	public:
		std::map<std::string,posVec> moves;
		/*! This function allows you to make a move in a one input game
		 * @brief Make one input move
		 * @param inputOne One input
		 * @param playerTurn Whose turn it is
		 * @return Has the game ended?
		 */
		virtual bool makeMove(posVec inputOne,bool playerTurn) { return false; } 

};
class twoMove : public baseGameLogic {
	public:
		std::map<std::string,moveInfo> moves;
		/*! This function allows you to make a move in a two input game
		 * @brief Make two input move
		 * @param inputOne First input
		 * @param inputTwo Second input
		 * @param playerTurn Whose turn it is
		 * @return Has the game ended?
		 */
		virtual bool makeMove(posVec inputOne,std::vector<posVec> inputTwo, bool playerTurn) { return false; }  

};



template <typename T> 
concept isOneMove = std::derived_from<T,oneMove>;

template <typename T> 
concept isTwoMove = std::derived_from<T,twoMove>;

template <typename T> 
concept usesTwoMoves = std::derived_from<T,twoMove>;

template <typename T>
concept usesMoves = std::derived_from<T,twoMove> || std::derived_from<T,oneMove>;

class chessLogic : public twoMove {
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
		int gameInt() override { return 2; }
	private:
		void changeMoves(bool playerTurn) override {};
};

class ticTacToeLogic : public oneMove {
	public:
		ticTacToeLogic();
		//ticTacToeLogic(gameInt::baseGameInt<ticTacToeLogic>& parent);
		bool isDuoMove() override { return false; }
		bool makeMove(posVec inputOne, bool playerTurn) override;
		int gameInt() override { return 0; }
		

		bool getWinner() override;
		//std::vector<std::vector<int>> * boardItems;
		/*
		 = {
    		{0, 0, 0},
    		{0, 0, 0},
    		{0, 0, 0}
		};
		*/
		//gameInt::baseGameInt<ticTacToeLogic>& parent;
	private:
		void changeMoves(bool playerTurn) override;
		bool checkForEnd();
		
};


class checkersLogic : public twoMove {
	public:
		checkersLogic();
		//checkersLogic(gameInt::baseGameInt<checkersLogic>& parent);
		bool makeMove(posVec inputOne,std::vector<posVec> inputTwo, bool playerTurn) override;
		
		bool isDuoMove() override { return true; }
		bool getWinner() override;
		//std::vector<std::vector<int>> boardItems;
		/*
		 = {
    		{0, 0, 0},
    		{0, 0, 0},
    		{0, 0, 0}
		};
		*/
		int gameInt() override { return 1; }
		//gameInt::baseGameInt<checkersLogic>& parent;
	private:
		void changeMoves(bool playerTurn) override;
		bool checkForEnd();
		std::string getPS(int x, int y);
	
		
};
//TODO temp remove this
#if 0
class connectLogic : public baseGameLogic {
	public:
		connectLogic();
		//connectLogic(gameInt::baseGameInt<connectLogic>& parent);
		bool isDuoMove() override { return false; }
		bool makeMove(posVec inputOne,bool playerTurn) override;
		bool makeMove(posVec inputOne,posVec inputTwo, bool playerTurn) override { return false ; };
		int gameInt() override { return 3; }
		

		bool getWinner() override;
		bool isOneD() override { return true; }
		//gameInt::baseGameInt<connectLogic>& parent
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
		/*!
		 * Get number of pieces in line starting at (cX,cY) and going in direction <vX,vY>
		 * @brief Get number of pieces in line
		 * @param cX Current X
		 * @param cY Current Y
		 * @param vX Vector X
		 * @param vY Vector Y
		 */
		int getLine(int cX, int cY, int vX, int vY);

		int lastX; //!< Last move X
		int lastY; //!< Last move Y
		
};
#endif
//TODO Change all cases to this
//! Checks if the type is a game
template <typename T> 
concept isGame = std::derived_from<T,game::baseGameLogic>;
}


