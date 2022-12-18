
#include <string>
#include <cmath>
#include <iostream>

#include "gameLogic.hpp"

const std::string tttRowNames[3] = {"top","center","lower"};
const std::string tttColNames[3] = {"left","middle","right"};

namespace game {

	bool ticTacToeLogic::checkForEnd() {
		for (int i = 0; i < 3; i++) {

		if (
			((this->boardItems[i][0] == this->boardItems[i][1]) && (this->boardItems[i][1] == this->boardItems[i][2]) && (!(this->boardItems[i][2] == 0))) ||
			((this->boardItems[0][i] == this->boardItems[1][i]) && (this->boardItems[1][i] == this->boardItems[2][i]) && (!(this->boardItems[2][i] == 0)))
		) { return true; }

		}

		if (
			((this->boardItems[0][0] == this->boardItems[1][1]) && (this->boardItems[1][1] == this->boardItems[2][2]) && (!(this->boardItems[2][2] == 0))) ||
			((this->boardItems[0][2] == this->boardItems[1][1]) && (this->boardItems[1][1] == this->boardItems[2][0]) && (!(this->boardItems[2][0] == 0)))
		) { return true; }

		if (
			(this->boardItems[0][0] != 0) &&
			(this->boardItems[0][1] != 0) &&
			(this->boardItems[0][2] != 0) &&
			(this->boardItems[1][0] != 0) &&
			(this->boardItems[1][1] != 0) &&
			(this->boardItems[1][2] != 0) &&
			(this->boardItems[2][0] != 0) &&
			(this->boardItems[2][1] != 0) &&
			(this->boardItems[2][2] != 0)
		) { return true; }

		return false;
	}



	ticTacToeLogic::ticTacToeLogic() {
		std::string nameArray[9];
		for (int i = 0; i < 3; i++) {
			for (int ii = 0; ii < 3; ii++) {
				nameArray[ii+(3*i)] = tttRowNames[i]+"-"+tttColNames[ii];
			}
		}
		
		this->moveNames = nameArray;
		for (int i = 0; i < 9; i++) {
			this->allowedMoves[i] = true;
		}
	}
	bool ticTacToeLogic::makeMove(int inputOne, int inputTwo) {
		auto getIntFromBool = [this]() {
			if (this->userTurn) {
				return 1;
			} else {
				return 2;
			}
		};

		this->boardItems[(inputOne%3)][int(std::floor(inputOne/3))] = getIntFromBool();
		this->allowedMoves[inputOne] = false;

		this->userTurn = (!(this->userTurn));

		return (this->checkForEnd());
	}










}