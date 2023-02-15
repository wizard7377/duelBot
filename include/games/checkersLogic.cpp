#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <cmath>
#include "gameLogic.hpp"
#include "utl.hpp"


template <typename T>
T & getValAt(int index, std::vector<std::vector<T>> * inVec) {
	return ((*inVec)[(int)(index % (*inVec)[0].size())][(int)(std::floor(index/(*inVec).size()))]);
}
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
		//std::cout << getValAt<int>(inputTwo,this->boardItems) << " to " <<  getValAt<int>(inputOne,this->boardItems) << std::endl;
		if (this->isCapture) { 
			std::vector<utl::point> * curVals = &(this->capMovesVec.at(inputTwo));
			this->boardItems->at(curVals->back().x)[curVals->back().y] = this->boardItems->at(curVals->front().x)[curVals->front().y];
			for (int i = 0; i < curVals->size() - 1; i++) {
				this->boardItems->at(curVals->at(i).x)[curVals->at(i).y] = 0;
				this->boardItems->at((int)((curVals->at(i).x+curVals->at(i+1).x)/2))[(int)((curVals->at(i).y+curVals->at(i+1).y)/2)] = 0;
			}


		} else {
			utl::point pOne = utl::point(inputOne,8,false);
			utl::point pTwo = utl::point(inputTwo,8,false);
			//std::cout << "inputs are: " << inputOne << ", " << inputTwo << std::endl;
			//std::cout << std::endl << "Imp val one: " << getValAt<int>(inputOne,this->boardItems) << std::endl << getValAt<int>(inputTwo,this->boardItems) << std::endl;
			getValAt<int>(inputTwo,this->boardItems) = getValAt<int>(inputOne,this->boardItems);
			getValAt<int>(inputOne,this->boardItems) = 0;
		}
		for (auto a : *(this->boardItems)) {
			for (auto b : a) {
				//std::cout << b;
			}
			//std::cout << std::endl;
		}
		this->changeMoves(playerTurn);
		return false;
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

int checkersLogic::pAt(int x, int y) {
	if ((x >= 0) and (x < 8) and (y >= 0) and (y < 8)) {
		return (this->boardItems->at(y).at(x));
	} else {
		return -1;
	}
}

void checkersLogic::changeMoves(bool playerTurn) {
	this->isCapture = false;
	
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
	//std::cout << spaces[0] << spaces[1] << std::endl;
	for (int i = 0; i < 8; i++) {
		for (int ii = 0; ii < 8; ii++) {
			//kings
			if 
			((this->pAt(i,ii) == pNames[0]) and
			(((this->pAt(i+1,ii+spaces[0]) == eNames[0]) and (this->pAt(i+2,ii+spaces[1]) == 0)) or 
			((this->pAt(i-1,ii+spaces[0]) == eNames[0]) and (this->pAt(i-2,ii+spaces[1]) == 0)))) 
			{
				this->isCapture = true;
				break;	
			}
		}
	}
	//std::cout << "isCapture is: " << this->isCapture << std::endl;
	if (this->isCapture) {
		std::vector<utl::point> relP;
		for (int i = 0; i < 8; i++) {
			for (int ii = 0; ii < 8; ii++) {
				//kings
				//std::cout << spaces[0] << spaces[1] << pNames[0] << pNames[1] << eNames[0] << eNames[1] << std::endl;
				if 
				((this->pAt(i,ii) == pNames[0]) and
				(((this->pAt(i+1,ii+spaces[0]) == eNames[0]) and (this->pAt(i+2,ii+spaces[1]) == 0)) or 
				((this->pAt(i-1,ii+spaces[0]) == eNames[0]) and (this->pAt(i-2,ii+spaces[1]) == 0)))) 
				{
					relP.push_back(utl::point(i,ii));
				}
			}
		}
		for (auto b : relP) {
			//std::cout << b.tString() << ",";
		}
		std::cout << std::endl;
		for (auto a : relP) {
			//std::string stringNow = checkRowNames[a.x] + checkColNames[a.y];
			std::string stringNow = "";
			std::vector<utl::point> curPoints;
			std::function<void(utl::point,std::string,std::vector<utl::point>)> spFunc = ([=,&a,&relP,&spFunc,this](utl::point inP,std::string curString,std::vector<utl::point> inPs) {
				//std::cout << curString << " with one " << inP.tString() << std::endl;
				//std::cout << curString << " with two " << inP.tString() << std::endl;

				inPs.push_back(inP);
				if ((a.x != inP.x) and (a.y != inP.y)) {
					curString = curString + "->" +  checkRowNames[inP.x] + checkColNames[inP.y];
					this->moveNames[a.x*8+a.y].push_back(curString);
					this->capMovesNames.push_back(curString);
					this->capMovesVec.push_back(inPs);
					this->capMoves.emplace(curString,inPs); 
				}
				if ((this->pAt(inP.x+1,inP.y+spaces[0]) == eNames[0]) and (this->pAt(inP.x+2,inP.y+spaces[1]) == 0)) {
					spFunc(utl::point(inP.x+2,inP.y+spaces[1]),curString,inPs);
				}
				if ((this->pAt(inP.x-1,inP.y+spaces[0]) == eNames[0]) and (this->pAt(inP.x-2,inP.y+spaces[1]) == 0)) {
					spFunc(utl::point(inP.x-2,inP.y+spaces[1]),curString,inPs);
				}
			});
			//std::cout << a.tString() << std::endl;
			spFunc(a,stringNow,curPoints);
			
			
		}
		


				

	} else {
		for (int i = 0; i < 8; i++) {
			for (int ii = 0; ii < 8; ii++) {
				
				if (this->pAt(i,ii) == pNames[0]) {
					//std::cout << "val is " << (i-1) << " ";
					if (this->pAt(i-1,ii+spaces[0]) == 0) {
						
						this->moveNames[i*8+ii].push_back(checkRowNames[i-1]+checkColNames[ii+spaces[0]]);
					}
					if (this->pAt(i+1,ii+spaces[0]) == 0) {
						this->moveNames[i*8+ii].push_back(checkRowNames[i+1]+checkColNames[ii+spaces[0]]);
					}
				}
			}
		}
	}

				


			
		
	
	



}
}
