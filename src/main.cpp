#include <dpp/dpp.h>
#include <map>
#include "colorstuff.hpp"
#include <variant>
#include <cctype>
#include <config.hpp>
#include "gameLogic.hpp"
#include "commandHandle.hpp"
#include "baseGameInt.hpp"
#include "databaselogic.hpp"
#include "frontend.hpp"
#include <iostream>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include "eventhandle.hpp"
#include "drawgame.hpp"
#include <cmath>
#include <chrono>  
#include <filesystem>
#include "ratesys.hpp"
#include <nlohmann/json.hpp>




using namespace dpp;
using json = nlohmann::json;


extern std::string getFullPath(std::string filePath);
extern std::string getFullPath(std::vector<std::string> filePath);
extern void handleChallengeSubmit(user userId, snowflake challengeId, std::string gameName, std::string guildName, cluster& bot,const form_submit_t& event,bool isRanked);
using snowPair = std::pair<snowflake,snowflake>;

/*
 * All the command handlers
 */

extern evt::eventhandle * handler;





int main(int argc, char *argv[]) {
	
	
	std::ifstream jFile(getFullPath("secrets/config.json"));
	json gameconfig = json::parse(jFile)["DISCORD"];
	
	cluster bot(gameconfig["BOT_TOKEN"]);
	

	
	try {
		handler = new evt::eventhandle(&bot);
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		exit(0);
	}

	
	
	
	bot.on_log(utility::cout_logger());

	if (argc != 0) {
		if (std::string(argv[0]) == "1") {
			
		}
	}
	
	

	
	
	

	bot.on_ready([&bot](const ready_t & event) {
        if (run_once<struct register_bot_commands>()) {
			bot.global_bulk_command_create({botCmds::infoDef(),botCmds::challengeDef(),botCmds::getRateDef(),botCmds::changeSetDef(),botCmds::joinQueueDef()});
			//bot.global_command_create(botCmds::infoDef());
			//bot.global_command_create(botCmds::challengeDef());
			//bot.global_command_create(botCmds::getRateDef());
			//bot.global_command_create(botCmds::changeSetDef());
			//bot.global_command_create(botCmds::joinQueueDef());
		}
    });
	
	
	
	bot.start(st_wait);
		

	



}
