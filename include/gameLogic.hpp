
#pragma once 

#include <string>
#include <vector>
#include <map>
#include <concepts>
#include "utl.hpp"

const std::string rowNames[] = {"a","b","c","d","e","f","g","h"};
const std::string colNames[] = {"1","2","3","4","5","6","7","8"};

std::string* getAllBoardNames();

namespace game {
class baseGameLogic {
	public:
		baseGameLogic();
		/*! Signals that moves involve more than two possible inputs */
		bool isCapture = false; 
		virtual bool isDuoMove() { return true; };
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
		/*! This function allows you to make a move in a two input game
		 * @brief Make two input move
		 * @param inputOne First input
		 * @param inputTwo Second input
		 * @param playerTurn Whose turn it is
		 * @return Has the game ended?
		 */
		virtual bool makeMove(int inputOne,int inputTwo, bool playerTurn) { return false; } 
		/*! This function allows you to make a move in a one input game
		 * @brief Make one input move
		 * @param inputOne One input
		 * @param playerTurn Whose turn it is
		 * @return Has the game ended?
		 */
		virtual bool makeMove(int inputOne,bool playerTurn) { return false; }
		std::string convertIntToString(int logicMove);
		int convertStringToInt(std::string userMove);
		/*! List of all possible moves, this the case of a single input game, 
		 * any vector that has a size of zero is invalid and greater than valid. 
		 * In a two player game, each inner vector repersents a possible first move, 
		 * and the elements repersent all possible second moves 
		 */
		std::vector<std::vector<std::string>> moveNames; 
		std::vector<std::vector<std::string>> moveNamesCon;
		std::vector<std::vector<std::string>> extraMoveNames;
		/*! Used only in games where it is possible to have more than two inputs per move, 
		 * map from second input to the list of moves 
		 */ 
		std::map<std::string,std::vector<utl::point>> capMoves; 
		
		std::vector<std::vector<utl::point>> capMovesVec; /*!< List of all possible moves with greater than 2 inputs */
		std::vector<std::string> capMovesNames; /*!< List of the repersentations of all moves with greater than 2 inputs*/
		virtual int gameInt() { return -1; };
	protected:

		virtual void changeMoves(bool playerTurn = false) {}; //!< Updates the list of moves
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
		int gameInt() override { return 2; }
	private:
		void changeMoves(bool playerTurn) override {};
};

class ticTacToeLogic : public baseGameLogic {
	public:
		ticTacToeLogic();
		bool isDuoMove() override { return false; }
		bool makeMove(int inputOne,bool playerTurn) override;
		bool makeMove(int inputOne,int inputTwo, bool playerTurn) override { return false ; };
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
		int gameInt() override { return 1; }
	private:
		void changeMoves(bool playerTurn) override;
		bool checkForEnd();
		int pAt(int x, int y); //!< Needed to turn natrual notation (x,y) into actual notation (y,x)
		void sAt(int x, int y, int piece); //!< Needed to turn natrual notation (x,y) into actual notation (y,x)
		
		
};
//TODO Change all cases to this
//! Checks if the type is a game
template <typename T> 
concept isGame = std::derived_from<T,game::baseGameLogic>;
}


