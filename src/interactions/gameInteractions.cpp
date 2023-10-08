#include "gameInteraction.hpp"
#include "gameLogic.hpp"
#include <string>
#include <functional>
#include <iostream>
#include <iterator>
#include <algorithm> 
#include <chrono>
#include <thread>
#include <random>
//TODO UNDO you know what to do
//template class gameInt::baseGameInt<game::baseGameLogic>;
template class gameInt::baseGameInt<game::ticTacToeLogic>;
//template class gameInt::baseGameInt<game::connectLogic>;
template class gameInt::baseGameInt<game::checkersLogic>;
//template class gameInt::baseGameInt<game::chessLogic>;

using namespace std::literals::chrono_literals;

namespace gameInt {

gameTimeType::gameTimeType(int seconds, int minutes, int hours) {
	this->seconds = seconds;
	this->minutes = minutes;
	this->hours = hours;

}

template <>
bool baseGameInt<game::ticTacToeLogic>::isDuoMove() { return false; }
template <>
bool baseGameInt<game::checkersLogic>::isDuoMove() { return true; }
template <typename T>
baseGameInt<T>::baseGameInt(const gameTime control[3], std::function<void(bool,int)> onEnd) {
	this->hasTimeLeft = new std::atomic<bool>(true);
	for (int i = 0; i < 3; i++) {
		this->timeControl[i] = new gameTime;
		*this->timeControl[i] = control[i];
	}
	this->lastMove = std::chrono::steady_clock::now();
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  	std::mt19937 generator (seed);  // mt19937 is a standard mersenne_twister_engine

	this->randomVal = generator() % 2;
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
int baseGameInt<T>::makeMove(bool playerTurn,std::string inputOne, std::string inputTwo) requires game::isOneMove<T> {
	
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
	isCase = (this->gameLogic->makeMove(this->fromMoveVec(inputOne),playerTurn));
	

	if (isCase) {
		//TODO MAKE END WORK
		this->endCase(!this->gameLogic->getWinner(),0);
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
int baseGameInt<T>::makeMove(bool playerTurn,std::string inputOne, std::string inputTwo) requires game::usesTwoMoves<T> {
	
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
	

	//BUG When ur wondering why this dosent work this is why 
	if (this->gameLogic->isCapture) {
		isCase = (this->gameLogic->makeMove(this->fromMoveVec(inputOne),this->toMoveVec(inputOne,inputTwo),playerTurn)); 
	} else {
		isCase = (this->gameLogic->makeMove(this->fromMoveVec(inputOne),this->toMoveVec(inputOne,inputTwo),playerTurn)); 
	}
	

	if (isCase) {
		//TODO MAKE END WORK
		this->endCase(!this->gameLogic->getWinner(),0);
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




/*
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
*/

template <typename T>
moveList baseGameInt<T>::getToMoves(std::string fromMove) 
requires game::usesTwoMoves<T> {
	moveList rList;
	for (const auto& [key, value] : this->gameLogic->moves[fromMove].first) {
		rList.push_back(key);
	}
	return rList;
}
template <typename T>
moveList baseGameInt<T>::getFromMoves() {
	moveList rList;
	for (const auto& [key, value] : this->gameLogic->moves) {
		rList.push_back(key);
	}
	return rList;
}


template <typename T>
posVec baseGameInt<T>::fromMoveVec(std::string fromMove) 
requires game::isOneMove<T> {
	return this->gameLogic->moves[fromMove];
}

template <typename T>
posVec baseGameInt<T>::fromMoveVec(std::string fromMove) 
requires game::usesTwoMoves<T> {
	return this->gameLogic->moves[fromMove].second;
}

template <typename T>
std::vector<posVec> baseGameInt<T>::toMoveVec(std::string fromMove, std::string toMove) 
requires game::usesTwoMoves<T> {
	return this->gameLogic->moves[fromMove].first[toMove];
}
template <typename T>
moveList baseGameInt<T>::getToMoves() 
requires game::usesTwoMoves<T> {
	//Probaly a smarter way to do this but idk
	moveList rList;
	for (const auto& [key, value] : this->gameLogic->moves) {
		for (const auto& [keyB, valueB] : value.first) {
			rList.push_back(keyB);
		}
		return rList;
	}
	return {};
}
}


