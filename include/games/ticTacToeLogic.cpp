
#include <string>
#include <cmath>
#include <iostream>

#include "gameLogic.hpp"

const std::string tttRowNames[3] = {"lower","center","top"};
const std::string tttColNames[3] = {"left","middle","right"};

namespace game {

	bool ticTacToeLogic::checkForEnd() {
		for (int i = 0; i < 3; i++) {

		if (
			(((*(this->boardItems))[i][0] == (*(this->boardItems))[i][1]) && ((*(this->boardItems))[i][1] == (*(this->boardItems))[i][2]) && (!((*(this->boardItems))[i][2] == 0))) ||
			(((*(this->boardItems))[0][i] == (*(this->boardItems))[1][i]) && ((*(this->boardItems))[1][i] == (*(this->boardItems))[2][i]) && (!((*(this->boardItems))[2][i] == 0)))
		) { return true; }

		}

		if (
			(((*(this->boardItems))[0][0] == (*(this->boardItems))[1][1]) && ((*(this->boardItems))[1][1] == (*(this->boardItems))[2][2]) && (!((*(this->boardItems))[2][2] == 0))) ||
			(((*(this->boardItems))[0][2] == (*(this->boardItems))[1][1]) && ((*(this->boardItems))[1][1] == (*(this->boardItems))[2][0]) && (!((*(this->boardItems))[2][0] == 0)))
		) { return true; }

		if (
			((*(this->boardItems))[0][0] != 0) &&
			((*(this->boardItems))[0][1] != 0) &&
			((*(this->boardItems))[0][2] != 0) &&
			((*(this->boardItems))[1][0] != 0) &&
			((*(this->boardItems))[1][1] != 0) &&
			((*(this->boardItems))[1][2] != 0) &&
			((*(this->boardItems))[2][0] != 0) &&
			((*(this->boardItems))[2][1] != 0) &&
			((*(this->boardItems))[2][2] != 0)
		) { return true; }

		return false;
	}



	ticTacToeLogic::ticTacToeLogic() {
		//I'm not dealing with this now
		std::vector<std::vector<std::string>> nameArray;
		this->boardItems = new std::vector<std::vector<int>>{{0,0,0},{0,0,0},{0,0,0}};

		//THE PREVIOUS DECLARATION IS CORRECT, THE NEXT IS ONLY FOR TESTING

		//this->boardItems = new std::vector<std::vector<int>>{{0,1,2},{2,0,1},{2,1,0}};

		for (int i = 0; i < 3; i++) {
			this->moveNamesCon.push_back({});
			for (int ii = 0; ii < 3; ii++) {
				nameArray.push_back({tttRowNames[i]+"-"+tttColNames[ii]});
				//this->moveNamesCon[i].push_back(tttRowNames[i]+"-"+tttColNames[ii]);
				this->moveNamesCon[i].push_back(tttRowNames[i]+"-"+tttColNames[ii]);
			}
		}
		
		
		this->moveNames = {};
		this->changeMoves(true);
		this->allowedMoves = new bool[9];
		for (int i = 0; i < 9; i++) {
			this->allowedMoves[i] = true;
		}

		
	}
	void ticTacToeLogic::changeMoves(bool playerTurn) {
		
		this->moveNames = {};
		for (int i = 0; i < 3; i++) {
			for (int ii = 0; ii < 3; ii++) {
				//if (this->boardItems->at(i)[ii] == 0) {
				if (this->boardItems->at(i)[ii] == 0) {
					this->moveNames.push_back({this->moveNamesCon[2-i][ii]});
				
				} else {
					this->moveNames.push_back({});
				}
				


			}
		}
		
		
		
	}

	bool ticTacToeLogic::makeMove(int inputOne,bool playerTurn) {
		auto getIntFromBool = [this]() {
			if (this->userTurn) {
				return 1;
			} else {
				return 2;
			}
		};

		//(*(this->boardItems))[(inputOne%3)][int(std::floor(inputOne/3))] = getIntFromBool();
		int cordOne = (inputOne%3);
		int cordTwo = int(std::floor(inputOne/3));
		(*(this->boardItems))[2-cordTwo][cordOne] = getIntFromBool();
		this->allowedMoves[inputOne] = false;

		this->userTurn = (!(this->userTurn));
		this->changeMoves(this->userTurn);

		return (this->checkForEnd());
	}

	bool ticTacToeLogic::getWinner() {
		for (int i = 0; i < 3; i++) {

		if (
			(((*(this->boardItems))[i][0] == (*(this->boardItems))[i][1]) && ((*(this->boardItems))[i][1] == (*(this->boardItems))[i][2]) && ((*(this->boardItems))[i][2] == 1)) ||
			(((*(this->boardItems))[0][i] == (*(this->boardItems))[1][i]) && ((*(this->boardItems))[1][i] == (*(this->boardItems))[2][i]) && ((*(this->boardItems))[2][i] == 1))
		) { return true; }

		}

		if (
			(((*(this->boardItems))[0][0] == (*(this->boardItems))[1][1]) && ((*(this->boardItems))[1][1] == (*(this->boardItems))[2][2]) && ((*(this->boardItems))[2][2] == 1)) ||
			(((*(this->boardItems))[0][2] == (*(this->boardItems))[1][1]) && ((*(this->boardItems))[1][1] == (*(this->boardItems))[2][0]) && ((*(this->boardItems))[2][0] == 1))
		) { return true; }


		return false;


}











}
