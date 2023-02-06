#pragma once

#include <map>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <unordered_map>
#include <typeindex>
#include "gameLogic.hpp"



using namespace dpp;

static std::map<std::string,std::string> gameEmojiName = {
    {"tictactoe","000"},
	{"checkers","001"}
};
struct gameConfig {
	std::string gameName;
	std::string gameId;
};

const std::unordered_map<std::type_index,gameConfig> gameCon {
	{std::type_index(typeid(game::ticTacToeLogic)),{"tictactoe","000"}},
	{std::type_index(typeid(game::checkersLogic)),{"checkers","001"}}
};




static std::map<std::string,std::map<std::string,std::vector<std::string>>> gamesToEmojis = 
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





static std::map<char,std::string> charToEmote = {
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

static std::string addChar(std::string inString) {
	while (inString.length() < 3) {
		inString = "0"+inString;
	}
	return inString;
}

static std::string reqId() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  	std::mt19937_64 generator (seed);
	return (std::to_string(generator()));

}













