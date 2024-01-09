#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <format>

#include "gameLogic.hpp"
#define BIG_VAL 1000000
template <typename T>
T getValAt(int index, std::vector<std::vector<T>> * inVec) {
	return (*inVec)[(int)(std::floor(index/(*inVec).size()))][(int)(index % (*inVec)[0].size())];
}
namespace game {



baseGameLogic::baseGameLogic() {
}

void baseGameLogic::printBoard() {
	std::cout << "\n\t\tGAME BEING PRINTED\n\n\n";
	for (int x = 0; x < this->boardItems->size() - 1; x++) {
		std::cout << "\t " << x << "  ";
		for (int y = 0; y < (*(this->boardItems))[x].size(); y++) {
			if (y != (*(this->boardItems))[x].size() - 1) std::cout << " " << (*this->boardItems)[x][y] << " |";
			else std::cout << " " << (*this->boardItems)[x][y];
		}
		std::cout << "\b\n";
		std::cout << "\t " << "   ";
		for (int y = 0; y < (*(this->boardItems))[x].size(); y++) {
			if (y != (*(this->boardItems))[x].size() - 1) std::cout << "---+";
			else std::cout << "---";
		}
		std::cout << "\n";
	}
	int xf = this->boardItems->size() - 1;
	std::cout << "\t " << xf << "  ";
	for (int y = 0; y < (*(this->boardItems))[xf].size(); y++) {
		if (y != (*(this->boardItems))[xf].size() - 1) std::cout << " " << (*this->boardItems)[xf][y] << " |";
		else std::cout << " " << (*this->boardItems)[xf][y];
	}
	std::cout << "\n\n";
	std::cout << "\t" << " x\n";
	std::cout << "\t" << "   y ";
	for (int y = 0; y < (*this->boardItems)[0].size(); y++) {
		std::cout << y << "   ";
	}
	std::cout << "\n\n\n\t\tEND GAME PRINT\n";
}


/*
std::string baseGameLogic::convertIntToString(int logicMove) {
	return (getValAt<std::string>(logicMove,(&this->moveNamesCon)));
}
*/
/*
int baseGameLogic::convertStringToInt(std::string userMove) {
	//TODO Make this less worse
	if (this->isOneD()) {
		int i = 0;
		for (i = 0; i < this->moveNames.size(); i++) {
			if (this->moveNames[i][0] == userMove) { return i; };
		}
		return i;
	} else {
		int i = 0;
		for (i = 0; i < this->moveNames.size(); i++) {
			if (this->convertIntToString(i) == userMove) { return i; };
		}
		return i;
	}
}
*/
//TODO Make this error handeling use C++ instead of C (why did i even do this)
int baseGameLogic::getAt(int x, int y) {
	if ((x < 0) or (y < 0)) return BIG_VAL; 
	if (this->boardItems->size() > x) {
		if (this->boardItems->at(x).size() > y) return (*(this->boardItems))[x][y];
		else return BIG_VAL;
	}
	else return BIG_VAL; 
}

}
