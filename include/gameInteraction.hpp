#pragma once

#include <string>
#include "gameLogic.hpp"
//#include "frontend.hpp"
#include <boost/chrono.hpp>
#include <vector>
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
using gameTime = std::chrono::duration<double,std::milli>;
using moveList = std::vector<std::string>;
//TODO make specific for each type of game so moves suck less
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
		baseGameInt(const gameTime control[3], std::function<void(bool,int)> onEnd);
		//baseGameInt(gameFront::baseGameHandle<T>& parent); 
		std::vector<std::vector<int>> getBoard();
		static_assert(std::is_base_of<game::baseGameLogic,T>::value, "Base game interactions may only have templates of game types");
		/**!
		 * Gets all possible moves in a two player game for a given first move
		 * @brief Gets all second moves
		 * @param fromMove First move
		 * @return Possible second moves
		*/
		moveList getToMoves(std::string fromMove) requires game::usesTwoMoves<T>;
		moveList getToMoves(std::string fromMove) requires game::isOneMove<T> { return {}; };
		/**!
		 * Gets all possible moves in a two player game
		 * @brief Gets all first moves
		 * @return Possible first moves
		*/
		moveList getFromMoves();
		/**!
		 * Changes a from move into its appropriate move vector
		 * @brief From string to appropriate vector
		 * @param fromMove Move string
		 * @return Move vector
		*/
		posVec fromMoveVec(std::string fromMove) requires game::isOneMove<T>;
		posVec fromMoveVec(std::string fromMove) requires game::usesTwoMoves<T>;
		/**!
		 * Changes a to move and its from move to its appropriate move vector
		 * @brief To string to appropriate vector
		 * @param toMove Second move string
		 * @param fromMove First move string
		 * @return Move vector
		*/
		std::vector<posVec> toMoveVec(std::string fromMove, std::string toMove) requires game::isOneMove<T> { return {}; }
		std::vector<posVec> toMoveVec(std::string fromMove, std::string toMove) requires game::usesTwoMoves<T>;
		/**!
		 * Gets default to moves
		 * @brief Gets all default second moves
		 * @return Possible second moves
		*/
		moveList getToMoves() requires game::usesTwoMoves<T>;
		moveList getToMoves() requires game::isOneMove<T> { return {}; } //Just to simplfy frontEnd stuff
		//W @Mishura4 For helping me figure this out
		int makeMove(bool playerTurn, std::string inputOne, std::string inputTwo = "") requires game::isOneMove<T>;
		int makeMove(bool playerTurn, std::string inputOne, std::string inputTwo = "") requires game::usesTwoMoves<T>;
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
		//gameFront::baseGameHandle<T>& parent; //!< IMPORTANT: object that created this object
	private:
		
		
		
		std::thread * timeThread; //!< Meseaures if time ran out
		std::atomic<bool> * hasTimeLeft; //!< If the game should end due to timeout
		bool userMove = true; //!< Tracks players turns
		//std::function<void(bool,int)> endCase;
};





}

		
