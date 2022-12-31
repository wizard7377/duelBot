#pragma once

#include <string>
#include <gameLogic.hpp>
#include <boost/chrono.hpp>
#include <vector>
#include <functional>
namespace gameInt {

class gameTimeType {
	public:
		gameTimeType(int seconds, int minutes = 0, int hours = 0);
		int hours, minutes, seconds;
};

template <typename T>
class baseGameInt {
	public:
		std::vector<std::vector<int>> getBoard();
		static_assert(std::is_base_of<game::baseGameLogic,T>::value, "Base game interactions may only have templates of game types");
		baseGameInt(gameTimeType* control[3], std::function<void(bool,int)> onEnd); 

		std::vector<std::vector<std::string>> getAllMoves();
		int makeMove(std::string inputOne, std::string inputTwo = "");

		gameTimeType * timeLeft[2];
		gameTimeType* timeControl[3];
		
	
	private:
		bool timeMove();
		boost::chrono::system_clock::time_point lastMove;
		T * gameLogic;
		bool userMove = true;
		std::function<void(bool,int)> endCase;
};

}

		
