#include "frontend.hpp"
#include <functional>
#include <map>
#include <string>
#include "baseGameInt.hpp"
#include "gameLogic.hpp"
#include <dpp/dpp.h>
using namespace dpp;

//template class gameInt::baseGameInt<game::baseGameLogic>;

template class gameFront::baseThread<game::ticTacToeLogic>;
template class gameFront::baseThread<game::baseGameLogic>;


std::map<std::string,std::string> gameEmojiName = {
    {"tictactoe","001"}
};

std::map<std::string,std::vector<std::string>> gamesToEmojis = {
	{"001",{"1057746629899341875","1057746631287640204","1057746632281694290"}}
};

std::string addChar(std::string inString) {
	while (inString.length() < 3) {
		inString = "0"+inString;
	}
	return inString;
}

std::map<char,std::string> charToEmote = {
	{'a',":regional_indicator_a:"},
	{'b',":regional_indicator_b:"},
	{'c',":regional_indicator_c:"},
	{'d',":regional_indicator_d:"},
	{'e',":regional_indicator_e:"},
	{'f',":regional_indicator_f:"},
	{'g',":regional_indicator_g:"},
	{'h',":regional_indicator_h:"},
	{'i',":regional_indicator_i:"},
	{'1',":one:"},
	{'2',":two:"},
	{'3',":three:"},
	{'4',":four:"},
	{'5',":five:"},
	{'6',":six:"},
	{'7',":seven:"},
	{'8',":eight:"},
	{'9',":nine:"}
};

namespace gameFront {

template <typename T>
baseThread<T>::baseThread(cluster* botPar, snowflake userIdA, snowflake userIdB, snowflake threadId, std::string gameName) {
	this->bot = botPar;
	this->userIdOne = userIdA;
	this->userIdTwo = userIdB;
	this->gameThread = threadId;
	this->emojiCode = gameEmojiName["tictactoe"];



	gameInt::gameTimeType* con[] = {new gameInt::gameTimeType(0,0,0),new gameInt::gameTimeType(0,0,0),new gameInt::gameTimeType(0,0,0)};
	this->gameInteraction = new gameInt::baseGameInt<T>(con,(std::bind(&baseThread::endCall,this,std::placeholders::_1,std::placeholders::_2)));
	//std::cout << this->gameThread << std::endl;

	

	message msg((this->gameThread),(this->drawBoard(true,this->gameInteraction->getBoard())));
	this->bot->message_create(msg);


}




template <typename T>
std::string baseThread<T>::drawBoard(bool userMove, std::vector<std::vector<int>> boardState) {
		
	std::string retString = "";
	
	char i = 'a';
	retString = retString + ":white_large_square:";
	
	for (auto c : boardState) {
	    retString = retString + charToEmote[i];
		//std::cout << std::to_string((int)(i)) << std::endl;
		i++;
	}
	retString = retString + '\n';
	
	i = '1';

	for (std::vector<int> a : boardState) {
		retString = retString + charToEmote[i];
		for (int b : a) {
			retString = retString + "<:" + (this->emojiCode)+"000"+addChar(std::to_string(b))+":"+gamesToEmojis[this->emojiCode][b] + ">";
		
		}
		i++;
		retString = retString + "\n";
	}
	std::cout << retString << std::endl;
	return retString;

		
}


}
