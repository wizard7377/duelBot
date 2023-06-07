#include "baseGameInt.hpp"
#include <string>
#include <functional>
#include <iostream>
#include <iterator>
#include <algorithm> 
#include <chrono>
#include <thread>

template class gameInt::baseGameInt<game::baseGameLogic>;
template class gameInt::baseGameInt<game::ticTacToeLogic>;
template class gameInt::baseGameInt<game::checkersLogic>;

using namespace std::literals::chrono_literals;

namespace gameInt {

gameTimeType::gameTimeType(int seconds, int minutes, int hours) {
	this->seconds = seconds;
	this->minutes = minutes;
	this->hours = hours;

}

template <typename T>
baseGameInt<T>::baseGameInt(const gameTime control[3], std::function<void(bool,int)> onEnd) {
	this->hasTimeLeft = new std::atomic<bool>(true);
	for (int i = 0; i < 3; i++) {
		this->timeControl[i] = new gameTime;
		*this->timeControl[i] = control[i];
	}
	this->lastMove = std::chrono::steady_clock::now();
	this->gameLogic = new T();
	
	//this->lastMove = boost::chrono::system_clock::now();
	this->timeLeft[0]=this->timeControl[0];
	this->timeLeft[1]=this->timeControl[0];

	this->endCase = onEnd;

}


template <typename T>
gameTime baseGameInt<T>::timeMove(bool userTime) {

	//auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    //std::cout << std::ctime(&timenow) << std::endl;


	gameTime timeTaken = std::chrono::steady_clock::now()-(this->lastMove);
	//std::cout << "(Bad) time is: " << (std::chrono::duration_cast<std::chrono::milliseconds>(timeTaken).count()) << std::endl;
	//std::cout << std::chrono::duration_cast<std::chrono::seconds>(timeTaken).count() << std::endl;
	//this->lastMove = std::chrono::steady_clock::now();
	if (timeTaken > *(this->timeControl[2])) {
		timeTaken = timeTaken - *(this->timeControl[2]);
	} else {
		timeTaken = 0s;
	}
	timeTaken = (timeTaken - *(this->timeControl[1]));
	if (userTime) {
		return(*(this->timeLeft[0]) - timeTaken);
	} else {
		return(*(this->timeLeft[1]) - timeTaken);
	}
}

template <typename T>
std::vector<std::vector<std::string>> baseGameInt<T>::getAllMoves() {
	return this->gameLogic->moveNames;
}
template <typename T> 
std::vector<std::vector<int>> baseGameInt<T>::getBoard() {
	//std::cout << "test" << std::endl;
	
	
	/*
	for (auto a : *(this->gameLogic->boardItems)) {
		
		for (int b : a) {
			std::cout << std::to_string(b);
		}
		std::cout << std::endl;
	}
	*/
	
	
	
	return *(this->gameLogic->boardItems);
}




template <typename T> 
int baseGameInt<T>::makeMove(bool playerTurn,std::string inputOne, std::string inputTwo) {
	gameTime * curTime = new gameTime; 
	*curTime = this->timeMove(this->userMove);
	this->timeThread = new std::thread([this,curTime] {
		std::this_thread::sleep_for(*curTime + (*this->timeControl[1]) + (*this->timeControl[2]));
		if (this->hasTimeLeft->load()) {
			this->hasTimeLeft->store(false);
			this->endCase(this->userMove,1);
		}
	});
	if (this->userMove) {
		(this->timeLeft[0]) = (curTime);
	} else {
		(this->timeLeft[1]) = (curTime);
	}
	bool isCase = false;

	if (inputTwo == "") { isCase = (this->gameLogic->makeMove(this->moveToInt(inputOne),playerTurn)); } 
	else {
		if (this->gameLogic->isCapture) {
			isCase = (this->gameLogic->makeMove(this->moveToInt(inputOne),this->moveToInt(inputTwo,true),playerTurn)); 
		} else {
			isCase = (this->gameLogic->makeMove(this->moveToInt(inputOne),this->moveToInt(inputTwo),playerTurn)); 
		}
	}
	this->lastMove = std::chrono::steady_clock::now();
	
	/*	
	if (this->timeMove()) {
		this->endCase(this->userMove,1);
	}

	if (isCase) {
		this->endCase(this->userMove,1);
	} else {
		this->userMove = !(this->userMove);
	}
	*/
	//this->endCase(this->userMove,-1);
	this->userMove = !(this->userMove);
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


