#if 0
#include <string>
#include <cmath>
#include <iostream>
#include <format>

#include "gameLogic.hpp"
//NOTE, (y,x)
namespace game {
//FIXME Connect four is 7 wide 6 high not the other way around

int connectLogic::getLine(int cX, int cY, int vX, int vY) {
	if ((cX+vX > 6) or (cX > 6)) return 1;
	else if ((cY+vY > 5) or (cY > 5)) return 1;
	else if (((*(this->boardItems))[cY][cX]) == ((*(this->boardItems))[cY+vY][cX+vX])) return (this->getLine(cX+vX,cY+vY,vX,vY) + 1);
	else return 1;
}
bool connectLogic::checkForEnd() {
	for (int i = -1; i <= 1; i++) {
		for (int ii = 0; ii <= 1; ii++) {
			if ((i != 0) or (ii != 0)) {
				std::cout << std::format("For line ({},{}), the line is: {}\n",i,ii,(this->getLine(this->lastX,this->lastY,i,ii) + this->getLine(this->lastX,this->lastY,-1 * i,-1 * ii)));
				if ((this->getLine(this->lastX,this->lastY,i,ii) + this->getLine(this->lastX,this->lastY,-1 * i,-1 * ii)) >= 5) return true; //three other pieces + (new piece counted twice)
																																			//Also only half are done because of negatvies
			}
		}
	}
	//TODO Make more efficent

	for (int i = 0; i < 7; i++) {
		for (int ii = 0; ii < 6; ii++) {
			//std::cout << std::to_string(i) << ", " << std::to_string(ii) << "\n";
			if ((*(this->boardItems))[ii][i] == 0) { return false; }
			
		}
		
	}
	return true;
}



connectLogic::connectLogic() {
	//Note, bot might just be unnexpectatly slow
	std::cout << __LINE__ << std::endl;
	//I'm not dealing with this now
	std::vector<std::vector<std::string>> nameArray;
	this->boardItems = new std::vector<std::vector<int>>(6,std::vector<int>(7));
	std::cout << __LINE__ << std::endl;
	//THE PREVIOUS DECLARATION IS CORRECT, THE NEXT IS ONLY FOR TESTING

	//this->boardItems = new std::vector<std::vector<int>>{{0,1,2},{2,0,1},{2,1,0}};

	for (int i = 1; i <= 7; i++) this->moveNames.push_back({std::to_string(i)});
	
	std::cout << __LINE__ << std::endl;
	
	this->changeMoves(true);
	this->allowedMoves = new bool[7];
	for (int i = 0; i < 7; i++) {
		this->allowedMoves[i] = true;
	}
	std::cout << __LINE__ << std::endl;

	
}
//TODO Get cord system straight
//playerTurn dosen't matter here
void connectLogic::changeMoves(bool playerTurn) {
	
	this->moveNames = {};
	for (int i = 0; i < 7; i++) {
		if ((*this->boardItems)[0][i] == 0) this->moveNames.push_back({std::to_string(i + 1)});
		else this->moveNames.push_back({""});


	}
	this->moveNamesCon = this->moveNames;
	
}
//TODO fix mess related to player turns being ints bools and messed up
bool connectLogic::makeMove(int inputOne,bool playerTurn) {
	
	//Maybe some error handling?
	this->lastX = inputOne;
	for (int i = 0; i < 6; i++) {
		if ((*(this->boardItems))[i][this->lastX] == 0) this->lastY = i;
	}
	std::cout << std::format("This current input is: ({},{})\n",this->lastX,this->lastY);
	(*(this->boardItems))[this->lastY][this->lastX] = (int)(this->userTurn) + 1;
	if ((*(this->boardItems))[5][this->lastX] != 0) this->allowedMoves[this->lastX] = false; 
	this->userTurn = (!(this->userTurn));
	this->changeMoves(this->userTurn);

	return (this->checkForEnd());
}

bool connectLogic::getWinner() {
	if (((*this->boardItems)[this->lastY][this->lastX]) == 1) return false;	
	else return true;
}











}
#endif