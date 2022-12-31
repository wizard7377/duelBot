#include "baseGameInt.hpp"
#include <string>
#include <functional>
namespace gameInt {

gameTimeType::gameTimeType(int seconds, int minutes, int hours) {
	this->seconds = seconds;
	this->minutes = minutes;
	this->hours = hours;

}

template <typename T>
baseGameInt<T>::baseGameInt(gameTimeType* control[3], std::function<void(bool,int)> onEnd) {
	for (int i = 0; i < 3; i++) {
		this->timeControl[i] = control[i];
	}
	this->gameLogic = new T();
	//this->lastMove = boost::chrono::system_clock::now();
	this->timeLeft[0]=control[0];
	this->timeLeft[1]=control[0];

	this->endCase = onEnd;

}

template <>
baseGameInt<game::baseGameLogic>::baseGameInt(gameTimeType* control[3], std::function<void(bool,int)> onEnd) {
	for (int i = 0; i < 3; i++) {
		this->timeControl[i] = control[i];
	}
	this->gameLogic = new game::baseGameLogic();
	//this->lastMove = boost::chrono::system_clock::now();
	
	this->timeLeft[0]=control[0];
	this->timeLeft[1]=control[0];

	this->endCase = onEnd;

}

template <typename T>
bool baseGameInt<T>::timeMove() {return false;}


template <typename T> 
std::vector<std::vector<int>> baseGameInt<T>::getBoard() {
	return this->gameLogic.boardItems;
}

template <> 
std::vector<std::vector<int>> baseGameInt<game::baseGameLogic>::getBoard() {
	return this->gameLogic->boardItems;
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


