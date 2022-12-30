#include "frontend.hpp"
#include <dpp/dpp.h>
#include "baseGameInt.hpp"
#include "gameLogic.hpp"
#include <typeindex>
#include <typeinfo>
#include <map>
#include <functional>
#include <vector>

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
std::string baseThread<T>::drawBoard(bool userMove, std::vector<std::vector<int>> boardState) {
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
		retString = retString + '\n';
	}
	return retString;

	
}
	



template <typename T>
baseThread<T>::baseThread(cluster& botPar, snowflake userIdA, snowflake userIdB, snowflake threadId) {
	this->bot = botPar;
	this->userIdOne = userIdA;
	this->userIdTwo = userIdB;
	this->gameThread = threadId;
	this->emojiCode = gameEmojiName["tictactoe"];
	const int con[] = {0,0,0};
	this->gameInteraction = new gameInt::baseGameInt<T>(con,this->endCall);
	(this->bot).message_create((this->gameThread),(this->drawBoard(true,this->gameInteraction.getBoard())));
	


}

}
