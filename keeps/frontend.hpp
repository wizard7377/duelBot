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


namespace gameFront {



	
template <typename T>
class baseThread {
	public:
	    
	    baseThread(cluster* botPar, snowflake userIdA, snowflake userIdB, snowflake threadId, std::string gameName) {
		    this->bot = botPar;
		    this->userIdOne = userIdA;
		    this->userIdTwo = userIdB;
		    this->gameThread = threadId;
		    this->emojiCode = gameEmojiName["tictactoe"];

		    gameInt::gameTimeType gameTime[3] = { gameInt::gameTimeType(0,0,0),gameInt::gameTimeType(0,0,0),gameInt::gameTimeType(0,0,0) };


		    this->gameInteraction = new gameInt::baseGameInt<T>(gameTime,(std::bind(&baseThread::endCall,this,std::placeholders::_1,std::placeholders::_2)));

		    message msg((this->gameThread),(this->drawBoard(true,this->gameInteraction->getBoard())));
		    this->bot->message_create(msg);
		

	    }
	    static_assert(std::is_base_of<game::baseGameLogic,T>::value, "Base game interactions may only have templates of game types");
	    std::string drawBoard(bool userMove, std::vector<std::vector<int>> boardState) {
		std::string retString = "";
		char i = 'a';
		retString = retString + ":white_large_square:";
		for (auto c : boardState[0]) {
			retString = retString + charToEmote[std::to_string(i)];
			i++;
		}
		i = 1;

		for (std::vector<int> a : boardState) {
			retString = retString + charToEmote[std::to_string(i)];
			for (int b : a) {
				retString = retString + ":"+(this->emojiCode)+"000"+addChar(std::to_string(b))+":";
			
			}
			i++;
			retString = retString + "\n";
		}
		return retString;

		
	    }
	    std::function<void(message msg)> moveCall;
	    void endCall(bool userWin, int winType);

	private:
	    cluster* bot;
	    snowflake userIdOne;
	    snowflake userIdTwo;
	    snowflake gameThread;
	    std::string emojiCode;
	    gameInt::baseGameInt<T> * gameInteraction;


};



}
