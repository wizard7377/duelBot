#pragma once

#include <string>
#include <gameLogic.hpp>
#include <boost/chrono.hpp>
#include <vector>
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
using gameTime = std::chrono::duration<double,std::milli>;
namespace gameInt {

class gameTimeType {
	public:
		gameTimeType(int seconds, int minutes = 0, int hours = 0);
		int hours, minutes, seconds;
};

class wrapState {};

/**
 * Main layer between game and thread, it takes a template argument, that of the game that is played
 * @class baseGameInt
 * @brief Main layer between game and thread
 * @tparam T The type of game that will be played
*/
template <typename T>
class baseGameInt : public wrapState {
	public:
		std::vector<std::vector<int>> getBoard();
		static_assert(std::is_base_of<game::baseGameLogic,T>::value, "Base game interactions may only have templates of game types");
		baseGameInt(const gameTime control[3], std::function<void(bool,int)> onEnd); 
		std::string intToMove(int userMove);
		int moveToInt(std::string userMove,bool typeReq = false);
		std::vector<std::vector<std::string>> getAllMoves();
		int makeMove(bool playerTurn, std::string inputOne, std::string inputTwo = "");
		/*! Checks if game is one input or two input
		 * @brief Number of inputs
		 * @return Is two player game?
		*/
		bool isDuoMove(); //TODO Fix mess involved with this 
		bool curPlayer = true;

		gameTime * timeLeft[2]; //!< How much time is left per player
		gameTime * timeControl[3]; //!< The time control
		std::chrono::time_point<std::chrono::steady_clock> lastMove; //!< Time of the last move played
		/*! Times user time based off current time and lastMove
		 * @brief Times move
		 * @param userTime who to deduct time from (false => 1, true => 2)
		 * @return Time left
		*/
		gameTime timeMove(bool userTime);
		std::function<void(bool,int)> endCase; //!< Callback for end of game
		bool randomVal;
		
		T * gameLogic; //!< Actual logic for the game
	private:
		
		
		
		std::thread * timeThread; //!< Meseaures if time ran out
		std::atomic<bool> * hasTimeLeft; //!< If the game should end due to timeout
		bool userMove = true; //!< Tracks players turns
		//std::function<void(bool,int)> endCase;
};

}

		
