#include "baseGameInt.hpp"
#include <string>
#include <functional>
#include <iostream>

template class gameInt::baseGameInt<game::baseGameLogic>;
template class gameInt::baseGameInt<game::ticTacToeLogic>;
template class gameInt::baseGameInt<game::checkersLogic>;

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


template <typename T>
bool baseGameInt<T>::timeMove() {return false;}


template <typename T> 
std::vector<std::vector<int>> baseGameInt<T>::getBoard() {
	//std::cout << "test" << std::endl;
	
	
	/*
	for (auto a : *(this->gameLogic->boardItems)) {
		
		for (int b : a) {
			std::cout << std::to_string(b) << std::endl;
		}
	}
	*/
	
	return *(this->gameLogic->boardItems);
}




template <typename T> 
int baseGameInt<T>::makeMove(std::string inputOne, std::string inputTwo) {
	bool isCase = false;

	if (inputTwo == "") { isCase = (this->gameLogic->makeMove(std::stoi(inputOne))); } 
	else { isCase = (this->gameLogic->makeMove(std::stoi(inputOne),std::stoi(inputTwo))); }
	if (this->timeMove()) {
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


