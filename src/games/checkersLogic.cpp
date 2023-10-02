#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <cmath>
#include <format>
#include <utility>
#include "gameLogic.hpp"


enum checks {
	EMPTY = 0,
	WHITEPAWN = 1,
	WHITEKING = 2,
	BLACKPAWN = 3,
	BLACKKING = 4,
};

enum moveType {
	CANT_MOVE,
	CAN_MOVE,
	CAN_CAP,
};
const std::string checkRowNames[] = {"a","b","c","d","e","f","g","h"};
const std::string checkColNames[] = {"1","2","3","4","5","6","7","8"};

namespace game {
checkersLogic::checkersLogic() {

	this->boardItems = new std::vector<std::vector<int>>{
		{0,1,0,1,0,1,0,1},
		{1,0,1,0,1,0,1,0},
		{0,1,0,1,0,1,0,1},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{3,0,3,0,3,0,3,0},
		{0,3,0,3,0,3,0,3},
		{3,0,3,0,3,0,3,0}
	};

	/*
	this->boardItems = new std::vector<std::vector<int>>{
		{0,0,0,0,0,0,0,0},
		{0,0,1,0,0,0,1,0},
		{0,0,0,0,0,0,0,0},
		{0,0,1,0,1,0,1,0},
		{0,3,0,3,0,3,0,0},
		{0,0,0,0,0,0,0,0},
		{0,3,0,3,0,0,0,0},
		{0,0,0,0,0,0,0,0}
	};
	*/
	this->userTurn = !(this->userTurn);
	this->changeMoves(this->userTurn);

}

bool checkersLogic::getWinner() {
	return true;
}
bool checkersLogic::makeMove(posVec inputOne, std::vector<posVec> inputTwo, bool playerTurn) {

	return false;
}


void checkersLogic::changeMoves(bool playerTurn) {

	std::pair<int,int> pieceNames = playerTurn ? std::pair<int,int>(1,2) : std::pair<int,int>(3,4); // What pieces are yours
	std::pair<int,int> enemyNames = playerTurn ? std::pair<int,int>(3,4) : std::pair<int,int>(1,2); // What pieces are your enemies
	std::pair<int,int> jumpValues = playerTurn ? std::pair<int,int>(1,2) : std::pair<int,int>(-1,-2); // What spaces you can jump
	

	auto canMove = [&](posVec inVec) -> moveType {
		bool canMove = false;
		bool canCap = false;
		if ((this->getAt(inVec[1],inVec[0]) != pieceNames.first) and (this->getAt(inVec[1],inVec[0]) != pieceNames.second)) return CANT_MOVE;
		
		//Check if reg piece can move without capture
		if ((this->getAt(inVec[1]+jumpValues.first,inVec[0]+1) == 0) or (this->getAt(inVec[1]+jumpValues.first,inVec[0]-1) == 0)) canMove = true;

		//Check if king
		if (this->getAt(inVec[1],inVec[0] == pieceNames.second)) {
			
			//Check behind
			if ((this->getAt(inVec[1]-jumpValues.first,inVec[0]+1) == 0) or (this->getAt(inVec[1]-jumpValues.first,inVec[0]-1) == 0)) canMove = true;
		}
		
		//Clean up this mess latter	
		if ((this->getAt(inVec[1]+jumpValues.second,inVec[0]+2) == 0) and 
			((this->getAt(inVec[1]+jumpValues.first,inVec[0]+1) ==  enemyNames.first) or 
			 (this->getAt(inVec[1]+jumpValues.first,inVec[0]+1) == enemyNames.second)))	
				{ canCap = true; }
		if ((this->getAt(inVec[1]+jumpValues.second,inVec[0]-2) == 0) and 
			((this->getAt(inVec[1]+jumpValues.first,inVec[0]-1) ==  enemyNames.first) or 
			 (this->getAt(inVec[1]+jumpValues.first,inVec[0]-1) == enemyNames.second)))	
				{ canCap = true; }
		//Check if capture available
		//Check king
		if (this->getAt(inVec[1],inVec[0] == pieceNames.second)) {
			//Check if capture available
			if ((this->getAt(inVec[1]-jumpValues.second,inVec[0]+2) == 0) and 
				((this->getAt(inVec[1]-jumpValues.first,inVec[0]+1) ==  enemyNames.first) or 
				 (this->getAt(inVec[1]-jumpValues.first,inVec[0]+1) == enemyNames.second)))	
					{ canCap = true; }
			if ((this->getAt(inVec[1]-jumpValues.second,inVec[0]-2) == 0) and 
				((this->getAt(inVec[1]-jumpValues.first,inVec[0]-1) ==  enemyNames.first) or 
				 (this->getAt(inVec[1]-jumpValues.first,inVec[0]-1) == enemyNames.second)))	
					{ canCap = true; }
		}

		if (canCap) return CAN_CAP;
		else if (canMove) return CAN_MOVE;
		else return CANT_MOVE;
	};

	//Get list of all possible captures from a given square
	auto getCapAt = [&](posVec inVec, int dX, int dY) -> std::vector<posVec> {
		std::vector<posVec> rVec = {};
		//Clean up this mess latter	
		//Basically just check for everything if it's the same
		if ((this->getAt(inVec[1]+jumpValues.second,inVec[0]+2) == 0) and 
			((this->getAt(inVec[1]+jumpValues.first,inVec[0]+1) ==  enemyNames.first) or 
			 (this->getAt(inVec[1]+jumpValues.first,inVec[0]+1) == enemyNames.second)))	
				{ if ((dX != 1) or (dY != jumpValues.first)) rVec.push_back({inVec[1] + jumpValues.second,inVec[0] + 2}); }
		if ((this->getAt(inVec[1]+jumpValues.second,inVec[0]-2) == 0) and 
			((this->getAt(inVec[1]+jumpValues.first,inVec[0]-1) ==  enemyNames.first) or 
			 (this->getAt(inVec[1]+jumpValues.first,inVec[0]-1) == enemyNames.second)))	
				{ if ((dX != -1) or (dY != jumpValues.first)) rVec.push_back({inVec[1] + jumpValues.second,inVec[0] - 2}); }

		//Check if capture available
		//Check king
		if (this->getAt(inVec[1],inVec[0] == pieceNames.second)) {
			if ((this->getAt(inVec[1]-jumpValues.second,inVec[0]+2) == 0) and 
				((this->getAt(inVec[1]-jumpValues.first,inVec[0]+1) ==  enemyNames.first) or 
				 (this->getAt(inVec[1]-jumpValues.first,inVec[0]+1) == enemyNames.second)))	
					{ if ((dX != 1) or (dY != (-1) * jumpValues.first)) rVec.push_back({inVec[1] + jumpValues.second,inVec[0] + 2}); }
			if ((this->getAt(inVec[1]-jumpValues.second,inVec[0]-2) == 0) and 
				((this->getAt(inVec[1]-jumpValues.first,inVec[0]-1) ==  enemyNames.first) or 
				 (this->getAt(inVec[1]-jumpValues.first,inVec[0]-1) == enemyNames.second)))	
					{ if ((dX != -1) or (dY != (-1) * jumpValues.first)) rVec.push_back({inVec[1] + jumpValues.second,inVec[0] - 2}); }
			//Check if capture available
		}
		return rVec;
	};

	
		
	/*
	 * So how this is gonna work is gonna be pretty simple
	 * We check every square for possible moves until either we run out
	 * or we run into a capture
	 * we store these moves in a vector
	 * If we run into a capture, we clear the vector and continue with captures
	 */

	std::vector<std::pair<posVec,std::vector<posVec>>> pMoves;
	bool canCap = false;	
	for (int i = 0; i < 8; i++) {
		for (int ii = 0; ii < 8; ii++) {
			std::string startSquare = rowNames[ii] + colNames[i];
			this->moves.insert({startSquare, {{}, {i,ii}}});
			//Check if we can move
			auto moveAble = canMove({i,ii});
			//If we can capture
			if (moveAble == CAN_CAP) {
				//If this is just discovered, clear all moves
				if (!(canCap)) pMoves.clear();
				
				canCap = true;
				
				//Need to do this logic
			//If we can move and don't need to cdpture (yet)
			} else if ((moveAble == CAN_MOVE) and (!(canCap))) {
				if (this->getAt(ii + jumpValues.first, i + 1) == 0) 
					this->moves[startSquare].first.insert({ rowNames[ii + jumpValues.first] + colNames[i + 1], {{ii + jumpValues.first, i + 1}}});
				if (this->getAt(ii + jumpValues.first, i - 1) == 0) 
					this->moves[startSquare].first.insert({ rowNames[ii + jumpValues.first] + colNames[i - 1], {{ii + jumpValues.first, i - 1}}});
				if (this->getAt(ii,i) == pieceNames.second) {
					if (this->getAt(ii - jumpValues.first, i + 1) == 0) 
						this->moves[startSquare].first.insert({ rowNames[ii - jumpValues.first] + colNames[i + 1], {{ii - jumpValues.first, i + 1}}});
					if (this->getAt(ii - jumpValues.first, i - 1) == 0) 
						this->moves[startSquare].first.insert({ rowNames[ii - jumpValues.first] + colNames[i - 1], {{ii - jumpValues.first, i - 1}}});


				}
			}
			
		}
	}

	
}

}
