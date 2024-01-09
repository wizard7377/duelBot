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

std::string canStr(moveType inType) {
	switch (inType) {
		case CANT_MOVE:
			return "CANT_MOVE";
		case CAN_MOVE:
			return "CAN_MOVE";
		case CAN_CAP:
			return "CAN_CAP";
		default:
			return "";
	}
}
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
	this->printBoard();
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

//Current status, lambda isnt problem
void checkersLogic::changeMoves(bool playerTurn) {
	
	std::pair<int,int> pieceNames = playerTurn ? std::pair<int,int>(1,2) : std::pair<int,int>(3,4); // What pieces are yours
	std::pair<int,int> enemyNames = playerTurn ? std::pair<int,int>(3,4) : std::pair<int,int>(1,2); // What pieces are your enemies
	std::pair<int,int> jumpValues = playerTurn ? std::pair<int,int>(1,2) : std::pair<int,int>(-1,-2); // What spaces you can jump
	

	auto canMoveAt = [&](int pX, int pY,int dX, int dY) -> bool {
		return (this->getAt(pY+dY,pX+dX) == 0);	
	};
	auto canMove = [&](int pX, int pY) -> moveType {
		bool canMove = false;
		bool canCap = false;
		if ((this->getAt(pY,pX) != pieceNames.first) and (this->getAt(pY,pX) != pieceNames.second)) return CANT_MOVE;
		
		//Check if reg piece can move without capture
		if ((this->getAt(pY+jumpValues.first,pX+1) == 0) or (this->getAt(pY+jumpValues.first,pX-1) == 0)) canMove = true;

		//Check if king
		if (this->getAt(pY,pX == pieceNames.second)) {
			
			//Check behind
			if ((this->getAt(pY-jumpValues.first,pX+1) == 0) or (this->getAt(pY-jumpValues.first,pX-1) == 0)) canMove = true;
		}
		
		//Clean up this mess latter	
		if ((this->getAt(pY+jumpValues.second,pX+2) == 0) and 
			((this->getAt(pY+jumpValues.first,pX+1) ==  enemyNames.first) or 
			 (this->getAt(pY+jumpValues.first,pX+1) == enemyNames.second)))	
				{ canCap = true; }
		if ((this->getAt(pY+jumpValues.second,pX-2) == 0) and 
			((this->getAt(pY+jumpValues.first,pX-1) ==  enemyNames.first) or 
			 (this->getAt(pY+jumpValues.first,pX-1) == enemyNames.second)))	
				{ canCap = true; }
		//Check if capture available
		//Check king
		if (this->getAt(pY,pX == pieceNames.second)) {
			//Check if capture available
			if ((this->getAt(pY-jumpValues.second,pX+2) == 0) and 
				((this->getAt(pY-jumpValues.first,pX+1) ==  enemyNames.first) or 
				 (this->getAt(pY-jumpValues.first,pX+1) == enemyNames.second)))	
					{ canCap = true; }
			if ((this->getAt(pY-jumpValues.second,pX-2) == 0) and 
				((this->getAt(pY-jumpValues.first,pX-1) ==  enemyNames.first) or 
				 (this->getAt(pY-jumpValues.first,pX-1) == enemyNames.second)))	
					{ canCap = true; }
		}

		if (canCap) return CAN_CAP;
		else if (canMove) return CAN_MOVE;
		else return CANT_MOVE;
	};

	//Get list of all possible captures from a given square
	auto getCapAt = [&](int pX,int pY) -> std::vector<posVec> {
		std::vector<posVec> rVec = {};
		//Clean up this mess latter	
		//Basically just check for everything if it's the same
		if ((this->getAt(pY+jumpValues.second,pX+2) == 0) and 
			((this->getAt(pY+jumpValues.first,pX+1) ==  enemyNames.first) or 
			 (this->getAt(pY+jumpValues.first,pX+1) == enemyNames.second)))	
				{ rVec.push_back({pY + jumpValues.second,pX + 2}); }
		if ((this->getAt(pY+jumpValues.second,pX-2) == 0) and 
			((this->getAt(pY+jumpValues.first,pX-1) ==  enemyNames.first) or 
			 (this->getAt(pY+jumpValues.first,pX-1) == enemyNames.second)))	
				{ rVec.push_back({pY + jumpValues.second,pX - 2}); }

		//Check if capture available
		//Check king
		if (this->getAt(pY,pX == pieceNames.second)) {
			if ((this->getAt(pY-jumpValues.second,pX+2) == 0) and 
				((this->getAt(pY-jumpValues.first,pX+1) ==  enemyNames.first) or 
				 (this->getAt(pY-jumpValues.first,pX+1) == enemyNames.second)))	
					{ rVec.push_back({pY + jumpValues.second,pX + 2}); }
			if ((this->getAt(pY-jumpValues.second,pX-2) == 0) and 
				((this->getAt(pY-jumpValues.first,pX-1) ==  enemyNames.first) or 
				 (this->getAt(pY-jumpValues.first,pX-1) == enemyNames.second)))	
					{ rVec.push_back({pY + jumpValues.second,pX - 2}); }
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
			moveType freeMove = canMove(i,ii);
			if ((freeMove == CAN_CAP) and (canCap = false)) {
				canCap = true;
				pMoves.clear();
			} 
			if (freeMove == CAN_CAP) pMoves.push_back({{i,ii},getCapAt(i,ii)});
			else if ((freeMove != CANT_MOVE) and (!(canCap))) { 
				//If you can move
				
	
				//Just a list of checks
				

				if (canMoveAt(i,ii,1,jumpValues.first)) { pMoves.push_back({{i,ii},{{i+1,ii+jumpValues.first}}}); }
				if (canMoveAt(i,ii,-1,jumpValues.first)) { pMoves.push_back({{i,ii},{{i-1,ii+jumpValues.first}}}); }
				//Kings
				if (this->getAt(ii,i) == pieceNames.second) {
					if (canMoveAt(i,ii,1,-jumpValues.first)) { pMoves.push_back({{i,ii},{{i+1,ii-jumpValues.first}}}); }
					if (canMoveAt(i,ii,-1,-jumpValues.first)) { pMoves.push_back({{i,ii},{{i-1,ii-jumpValues.first}}}); }
				}
			}
		}
	}
	
	
	//Now we write it to our class variable...
	std::cout << "canCap is: " << canCap << std::endl;
	for (int i = 0; i < pMoves.size(); i++) {	
		if (canCap) {


		} else {
			std::string startMove = (std::to_string(pMoves[i].first[0]) + std::to_string(pMoves[i].first[1]));
			std::string endMove = (std::to_string(pMoves[i].second[0][0]) + std::to_string(pMoves[i].second[0][1]));
			//Forming capture string
			for (int j = 1; j < pMoves[i].second.size(); j++) {
				endMove += "->" + (std::to_string(pMoves[i].second[j][0]) + std::to_string(pMoves[i].second[j][1]));
			}
			std::map<std::string,std::vector<posVec>> tMoves {{ endMove, pMoves[i].second }};
			this->moves.insert({startMove,{tMoves,pMoves[i].first}});

			//When I continue with this:
			//I need to print all these to the class variable, and remeber, in pMoves every single in move is given mutiple slots if it has mutiple out moves
		}
	}	
	
}

}
