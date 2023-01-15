#include "frontend.hpp"
#include <functional>
#include <map>
#include <iterator>
#include <string>
#include "baseGameInt.hpp"
#include "gameLogic.hpp"
#include <dpp/dpp.h>
#include <algorithm>
#include "drawgame.hpp"
using namespace dpp;

//template class gameInt::baseGameInt<game::baseGameLogic>;

template class gameFront::baseThread<game::ticTacToeLogic>;
template class gameFront::baseThread<game::checkersLogic>;
template class gameFront::baseThread<game::baseGameLogic>;


std::map<std::string,std::string> gameEmojiName = {
    {"tictactoe","000"},
	{"checkers","001"}
};




std::map<std::string,std::map<std::string,std::vector<std::string>>> gamesToEmojis = 
{
	{"000",{{"000",{"1057746629899341875","1057746631287640204","1057746632281694290"}}}},

	{"001",
	{
		{"000",
			{"1062886057563127839","1062886058473296022","1062886059677065267","1062886060566249472","1062886061551919104"}
		},

		{"001",
			{"1062886062378205295"}
		}
	
	}}
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
	this->emojiCode = gameEmojiName[gameName];
	this->gameDraw = new dg::basicDrawGame(gameName);



	gameInt::gameTimeType* con[] = {new gameInt::gameTimeType(0,0,0),new gameInt::gameTimeType(0,0,0),new gameInt::gameTimeType(0,0,0)};
	this->gameInteraction = new gameInt::baseGameInt<T>(con,(std::bind(&baseThread::endCall,this,std::placeholders::_1,std::placeholders::_2)));
	//std::cout << this->gameThread << std::endl;

	
	std::string imgPath = this->gameDraw->getBoard(this->gameInteraction->getBoard());

	
	//do more config stuff later
	//+ stuff with sync
	embed mainEmb = embed().
		set_color(colors::blue_aquamarine).
		set_title("Move: of the game between: " + bot->user_get_sync(userIdA).get_mention() + " and " + bot->user_get_sync(userIdA).get_mention()).
		set_author("Duel Bot","https://github.com/wizard7377/duelBot.git",(bot->current_user_get_sync().get_avatar_url())).
		set_image("attachment://game.png");

	message msg((this->gameThread),mainEmb);
	msg.add_file("game.png",utility::read_file(imgPath));	
	this->bot->message_create(msg);
	std::filesystem::remove(imgPath);
	
	
	


}




template <typename T>
std::string baseThread<T>::drawBoard(bool userMove, std::vector<std::vector<int>> boardState) {
		
	
	
	char i = 'a';
	std::string retString = ":white_large_square:";

	
	for (auto c : boardState) {
	    retString = retString + charToEmote[i];
		//std::cout << std::to_string((int)(i)) << std::endl;
		i++;
	}
	retString = retString + '\n';
	

	
	i = '1';

	for (auto a : boardState) {
		retString = retString+ charToEmote[i];
		for (int b : a) {
			retString= retString+ "<:" + (this->emojiCode)+"000"+addChar(std::to_string(b))+":"+gamesToEmojis[this->emojiCode]["000"][b] + ">";
		
		}
		i++;
		retString = retString + '\n';
	}
	
	std::cout << retString;
	return retString;

		
}


}
