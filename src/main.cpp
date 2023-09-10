
/**
 * @file main.cpp
 * @copyright Wizard and Co. 2023, Don't steal it (please and thank you (this is a test (doc)))
 */


#include <dpp/dpp.h>
#include <map>
#include "colorUtil.hpp"
#include <variant>
#include <spdlog/spdlog.h>
#include <cctype>
#include "gameLogic.hpp"
#include "commandHandler.hpp"
#include "gameInteraction.hpp"
#include "databaseLogic.hpp"
#include "frontend.hpp"
#include <iostream>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include "eventHandler.hpp"
#include "drawGame.hpp"
#include <cmath>
#include <chrono>  
#include <filesystem>
#include "rateQueue.hpp"
#include <nlohmann/json.hpp>




//TODO Add more detailed docs so you can remeber things
using namespace dpp;
using json = nlohmann::json;

#ifndef TOKEN_TYPE
	#define TOKEN_TYPE "BUILD"
#endif
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
	json gameconfig = json::parse(jFile)[TOKEN_TYPE]["DISCORD"];
	spdlog::set_pattern("(DuelBot) [%T] %^%l%$: %v");
	if (TOKEN_TYPE == "TEST") spdlog::warn("Using test token");
	cluster bot(gameconfig["BOT_TOKEN"]);
	
	
	try {
		handler = new evt::eventhandle(&bot);
	} catch (const std::exception& e) {
		spdlog::critical("Event handle failed with: {}", e.what());
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
