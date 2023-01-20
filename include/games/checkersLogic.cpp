#include <vector>
#include <iostream>
#include <string>
#include "gameLogic.hpp"



enum checks {
	EMPTY = 0,
	WHITEPAWN = 1,
	WHITEKING = 2,
	BLACKPAWN = 3,
	BLACKKING = 4,
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

		
		this->moveNames = std::vector<std::vector<std::string>>();
		
		for (int i = 0; i < 8; i++) {
			this->moveNamesCon.push_back({});
			for (int ii = 0; ii < 8; ii++) {
				this->moveNames.push_back({});
				this->moveNamesCon[i].push_back(checkRowNames[i]+checkColNames[ii]);
			}
		}
			/*
			for (int ii = 0; ii < 8; ii++) {
				this->moveNames[i].push_back(checkRowNames[i]+checkColNames[ii]);
			
			}
			*/
		
		this->changeMoves(true);


	
	}
	bool checkersLogic::makeMove(int inputOne,int inputTwo,bool playerTurn) {
		this->changeMoves(playerTurn);
		return true;
	}
	bool checkersLogic::getWinner() { return true; }


int rUp(int inputOne,int cel = 8) {
	if (inputOne > 0) {
		if (inputOne < cel) {
			return inputOne;
		} else {
			return cel;
		}
	} else {
		return 0;
	}
}


void checkersLogic::changeMoves(bool playerTurn) {
	bool isCapture = false;
	
	int *pNames;
	int *eNames;
	int *spaces;

	if (playerTurn) {
		pNames = new int[2] {1,2};
		eNames = new int[2] {3,4};
		spaces = new int[2] {1,2};
	} else {
		pNames = new int[2] {3,4};
		eNames = new int[2] {1,2};
		spaces = new int[2] {-1,-2};
	}
	for (int i = 0; i < 8; i++) {
		for (int ii = 0; ii < 8; ii++) {
			std::cout << std::to_string(this->boardItems->at(i)[ii]) << " Compared to: " << std::to_string(pNames[0]) << " Which is: " << ((this->boardItems->at(i)[ii]) == pNames[0]) << " at: " << checkRowNames[i] << checkColNames[ii] << std::endl;
			if ((this->boardItems->at(i)[ii]) == pNames[0]) {
				
				if (((this->boardItems->at(rUp(i+1))[rUp(ii+spaces[1])]) == 0) || ((this->boardItems->at(rUp(i-1))[rUp(ii+spaces[1])]) == 0)) { 
					if (((this->boardItems->at(rUp(i+1))[rUp(ii+spaces[0])]) == 0)) {
						this->moveNames[8*i+ii].push_back(checkRowNames[rUp(i+1)]+checkColNames[rUp(ii+spaces[0])]);
					}
					if ((this->boardItems->at(rUp(i-1))[rUp(ii+spaces[0])]) == 0) {
						this->moveNames[8*i+ii].push_back(checkRowNames[rUp(i-1)]+checkColNames[rUp(ii+spaces[0])]);
					}
				}
				if (((this->boardItems->at(rUp(i+1))[rUp(ii+spaces[0])]) == eNames[0]) || ((this->boardItems->at(rUp(i-1))[rUp(ii+spaces[0])]) == eNames[0])) { 
						this->moveNames.clear();
						isCapture = true;
						break;
				}
				else if ((this->boardItems->at(rUp(i+1))[rUp(ii+spaces[0])] == 0) || (this->boardItems->at(rUp(i-1))[rUp(ii+spaces[0])] == 0)) { 
					std::cout << "check point one";
					if (this->boardItems->at(rUp(i-1))[rUp(ii+spaces[0])] == 0) {
						this->moveNames[8*i+ii].push_back(checkRowNames[rUp(i+1)]+checkColNames[rUp(ii+spaces[0])]);
					}
					if (this->boardItems->at(rUp(i-1))[rUp(ii+spaces[0])] == 0) {
						this->moveNames[8*i+ii].push_back(checkRowNames[rUp(i-1)]+checkColNames[rUp(ii+spaces[0])]);
					}
				}
			}
		}
	}
	int i = 0;
	int ii = 0;
	/*
	for (auto a : *(this->boardItems)) {
		for (auto b : a) {
			
			std::cout << " ( " << b << " : " << checkRowNames[i] << checkColNames[ii] << " ) " ;
			i++;
		}
		i = 0;
		std::cout << std::endl;
		ii++;
	}
	*/
	/*
	std::cout << "Wizard goffy (We\'re doing this again... seriously.) 1" << std::endl;
	
	for (auto a : this->moveNames) {
		for (auto b : a) {
			std::cout << b << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << "Wizard goffy (We\'re doing this again... seriously.) 2" << std::endl;
	*/



}
}
