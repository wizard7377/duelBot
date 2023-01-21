#include "baseGameInt.hpp"
#include <string>
#include <functional>
#include <iostream>
#include <iterator>
#include <algorithm> 

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
std::vector<std::vector<std::string>> baseGameInt<T>::getAllMoves() {
	return this->gameLogic->moveNames;
}
template <typename T> 
std::vector<std::vector<int>> baseGameInt<T>::getBoard() {
	//std::cout << "test" << std::endl;
	
	
	
	for (auto a : *(this->gameLogic->boardItems)) {
		
		for (int b : a) {
			std::cout << std::to_string(b);
		}
		std::cout << std::endl;
	}
	
	
	
	return *(this->gameLogic->boardItems);
}




template <typename T> 
int baseGameInt<T>::makeMove(bool playerTurn,std::string inputOne, std::string inputTwo) {
	bool isCase = false;

	if (inputTwo == "") { isCase = (this->gameLogic->makeMove(this->moveToInt(inputOne),playerTurn)); } 
	else {
		if (this->gameLogic->isCapture) {
			isCase = (this->gameLogic->makeMove(this->moveToInt(inputOne),this->moveToInt(inputTwo,true),playerTurn)); 
		} else {
			isCase = (this->gameLogic->makeMove(this->moveToInt(inputOne),this->moveToInt(inputTwo),playerTurn)); 
		}
	}
	
		
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

template <typename T> 
std::string baseGameInt<T>::intToMove(int userMove) {
	return (this->gameLogic->convertIntToString(userMove));
}
template <typename T> 
bool baseGameInt<T>::isDuoMove() {
	return (this->gameLogic->isDuoMove());
}
template <typename T> 
int baseGameInt<T>::moveToInt(std::string userMove, bool typeReq) {
	if (typeReq) {
		return (std::distance(this->gameLogic->capMovesNames.begin(),std::find(this->gameLogic->capMovesNames.begin(),this->gameLogic->capMovesNames.end(),userMove)));
	} else {
		return (this->gameLogic->convertStringToInt(userMove));
	}	
}


}


