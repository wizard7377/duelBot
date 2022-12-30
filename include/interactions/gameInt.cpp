#include "baseGameInt.hpp"
#include <string>
#include <functional>
namespace gameInt {

template <typename T>
bool baseGameInt<T>::timeMove() {return false;}

template <typename T>
baseGameInt<T>::baseGameInt(gameTimeType control[3], std::function<void(bool,int)> onEnd) {
	this->timeControl = control;
	this->gameLogic = new T();
	this->lastMove = boost::chrono::system_clock::now();
	this->timeLeft = {control[0],control[0]};

	this->endCase = onEnd;

}

template <typename T> 
std::vector<std::vector<int>> baseGameInt<T>::getBoard() {
	return this->gameLogic.boardItems;
}

template <typename T> 
int baseGameInt<T>::makeMove(std::string inputOne, std::string inputTwo) {
	bool isCase = false;
	if (inputTwo == "") { isCase = (this->gameLogic.makeMove(inputOne)); } 
	else { isCase = (this->gameLogic.makeMove(inputOne,inputTwo)); }
	if (this->timeMove) {
		this->endCase(this->userMove,2);
	}

	if (isCase) {
		this->endCase(this->userMove,1);
	} else {
		this->userMove = !(this->userMove);
	}
	return 1;
}


}


