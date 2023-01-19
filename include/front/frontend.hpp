#pragma once

#include <dpp/dpp.h>
#include "baseGameInt.hpp"
#include "gameLogic.hpp"
#include <typeindex>
#include <typeinfo>
#include <string>
#include <map>
#include <iostream>
#include <atomic>
#include <functional>
#include "drawgame.hpp"

using namespace dpp;

namespace gameFront {


class wrapThread {};



	
template <typename T>
class baseThread : public wrapThread {
	public:
	    
	    baseThread(cluster* botPar, snowflake userIdA, snowflake userIdB, snowflake threadId, std::string gameName);
	    //~baseThread();
		static_assert(std::is_base_of<game::baseGameLogic,T>::value, "Base game interactions may only have templates of game types");
		std::string drawBoard(bool userMove, std::vector<std::vector<int>> boardState);
	    std::function<void(message msg)> moveCall;
	    void endCall(bool userWin, int winType) {
			std::cout << "game has ended :(" << std::endl;
		}

	private:
	    cluster* bot;
	    snowflake userIdOne;
	    snowflake userIdTwo;
	    snowflake gameThread;
		channel gameThreadObj;
	    std::string emojiCode;
		dg::baseDrawGame * gameDraw;
	    gameInt::baseGameInt<T> * gameInteraction;
		event_handle buttonEventId;
		event_handle selectEventId;
		std::map<std::string,std::function<void(const button_click_t&)>> gameButtonCmds;
		std::map<std::string,std::function<void(const select_click_t&)>> gameSelectCmds;


};



}
