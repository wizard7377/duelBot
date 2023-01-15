#pragma once

#include <dpp/dpp.h>
#include "baseGameInt.hpp"
#include "gameLogic.hpp"
#include <typeindex>
#include <typeinfo>
#include <string>
#include <map>
#include <iostream>
#include <functional>

using namespace dpp;


/*
std::map<std::string,std::string> gameEmojiName = {
    {"tictactoe","001"}
};

std::string addChar(std::string inString) {
	while (inString.length() < 3) {
		inString = "0"+inString;
	}
	return inString;
}

std::map<std::string,std::string> charToEmote = {
	{"a",":regional_indicator_a:"},
	{"b",":regional_indicator_b:"},
	{"c",":regional_indicator_c:"},
	{"d",":regional_indicator_d:"},
	{"e",":regional_indicator_e:"},
	{"f",":regional_indicator_f:"},
	{"g",":regional_indicator_g:"},
	{"h",":regional_indicator_h:"},
	{"i",":regional_indicator_i:"},
	{"1",":one:"},
	{"2",":two:"},
	{"3",":three:"},
	{"4",":four:"},
	{"5",":five:"},
	{"6",":six:"},
	{"7",":seven:"},
	{"8",":eight:"},
	{"9",":nine:"}
};
*/

namespace gameFront {


class wrapThread {};



	
template <typename T>
class baseThread : public wrapThread {
	public:
	    
	    baseThread(cluster* botPar, snowflake userIdA, snowflake userIdB, snowflake threadId, std::string gameName);
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
	    gameInt::baseGameInt<T> * gameInteraction;


};



}
