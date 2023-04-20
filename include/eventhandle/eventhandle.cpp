#include <thread>
#include <string>
#include <dpp/dpp.h>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <concepts>
#include "colorstuff.hpp"
#include "gamenums.hpp"
#include "frontend.hpp"
//#include "ratesys.hpp"
#include "eventhandle.hpp"

/*
CHANGE SCOPE OF TRY BLOCKS
CHANGE SCOPE OF TRY BLOCKS
CHANGE SCOPE OF TRY BLOCKS
*/



using namespace dpp;

extern void handleChallengeSubmit(user userId, snowflake challengeId, std::string gameName, std::string guildName, cluster& bot,const form_submit_t& event,bool isRanked);
extern bool checkExists(snowflake userOne, snowflake userTwo);
namespace evt {


/***
 * @brief Generates a function required for the front end queue (yes i know its a template function (a function that generates a function) generating a function which is in of itself a template function)
 * @param botPar global bot
 * @param inTimes all the times
*/
template <typename T>
//requires std::derived_from<T,game::baseGameLogic>
std::function<void(snowPair,snowPair)> makeQ(cluster* botPar,std::vector<gameTime> inTimes,std::function<gameFront::baseSimThread<T>*(gameInt::baseGameInt<T>*,snowPair,snowPair)> inFuncs[2],eventhandle * inEvent) {
	
	return ([botPar,inTimes,inFuncs,inEvent](snowPair players,snowPair threads) {
		gameFront::baseGameHandle<T> * cHand;
		gameInt::baseGameInt<T> * gInt;
		//Create gameInt and gameHandle, however because there interdependent i need to do this
		gInt = new gameInt::baseGameInt<T>(inTimes.data(),
		([cHand](bool winner, int winCase) {
			//std::cout << "game has ended :((\n";
			//std::cout << "At line: " << colorForm(std::to_string(__LINE__),RED_TERM) << std::endl;
			setRates(cHand->gameHandles[0]->handler->testCon,cHand->gameHandles[0]->userIdOne,cHand->gameHandles[0]->userIdTwo,0,winner); // weeeeeeeeeee very lazy
			//std::cout << "At line: " << colorForm(std::to_string(__LINE__),RED_TERM) << std::endl;
			setRates(cHand->gameHandles[1]->handler->testCon,cHand->gameHandles[1]->userIdOne,cHand->gameHandles[1]->userIdTwo,0,!winner); // weeeeeeeeeee very lazy
			//std::cout << "At line: " << colorForm(std::to_string(__LINE__),RED_TERM) << std::endl;
			cHand->gameHandles[0]->endCall(winner,winCase);
			cHand->gameHandles[1]->endCall(!winner,winCase);
		})); 
		//Create game handle and such threads
		std::function<gameFront::inType*(gameInt::baseGameInt<T>*)> inFuncs[2] = {
			[&](gameInt::baseGameInt<T>* gInt) { return new gameFront::baseSimThread<T>(botPar,players.first,players.second,threads.first,inEvent,gInt); },
			[&](gameInt::baseGameInt<T>* gInt) { return new gameFront::baseSimThread<T>(botPar,players.second,players.first,threads.second,inEvent,gInt); }
		};
		cHand = new gameFront::baseGameHandle<T>(
			inFuncs,
			inTimes
		);
		botPar->thread_member_add(threads.first,players.first);
		botPar->thread_member_add(threads.second,players.second);
	});
}


eventhandle::eventhandle(cluster * bot) {

	this->testCon = new mData::dataHandle();
	std::function<gameFront::baseSimThread<game::ticTacToeLogic>*(gameInt::baseGameInt<game::ticTacToeLogic>*,snowPair, snowPair)> tttFuncs[2] = {
		([&] (gameInt::baseGameInt<game::ticTacToeLogic>* inState,snowPair playId, snowPair threadId) { return new gameFront::baseSimThread<game::ticTacToeLogic>(bot,playId.first,playId.second,threadId.first,this,inState); })
		,
		([&] (gameInt::baseGameInt<game::ticTacToeLogic>* inState,snowPair playId, snowPair threadId) { return new gameFront::baseSimThread<game::ticTacToeLogic>(bot,playId.second,playId.first,threadId.second,this,inState); })
	};
	//std::vector<rQ::frontRQ> tttQS;

		
	//tttQS.emplace_back(rQ::frontRQ( makeQ<game::ticTacToeLogic>(bot,{5min,5s,0s},tttFuncs,this)));
	//tttQS.emplace_back(rQ::frontRQ( makeQ<game::ticTacToeLogic>(bot,{5min,0s,0s},tttFuncs,this)));
	
	//this->curQueues.emplace_back(tttQS);
	std::vector<gameTime> curTimes {5min, 5s, 0s};
	this->curQueues[0].push_back(new rQ::frontRQ(makeQ<game::ticTacToeLogic>(bot, curTimes, tttFuncs, this),this->testCon));
	std::vector<gameTime> curTimesTwo {5min, 5s, 0s};
	this->curQueues[0].push_back(new rQ::frontRQ(makeQ<game::ticTacToeLogic>(bot, curTimesTwo, tttFuncs, this),this->testCon));	
	std::vector<gameTime> curTimesThree {1min, 0s, 0s};
	this->curQueues[0].push_back(new rQ::frontRQ(makeQ<game::ticTacToeLogic>(bot, curTimesThree, tttFuncs, this),this->testCon));	
	
	
	
	bot->on_select_click([this](const auto& event) {
		try {
			std::thread([this,event] { try { this->selectCmds.at(event.custom_id)(event); } catch (...) {} } ).detach();
		} catch (...) {
			std::cout << "An error has occured" << std::endl;
		}
	});
	bot->on_button_click([this](const auto& event) {
		try {
			std::thread([this,event] { (this->buttonCmds.at(event.custom_id))(event); }).detach();
		} catch (...) {
			std::cout << "An error has occured" << std::endl;
		}
	});
	bot->on_form_submit([this](const auto& event) {
		try {
			std::thread([this,event] { this->formCmds.at(event.custom_id)(event); }).detach();
		} catch (...) {
			std::cout << "An error has occured" << std::endl;
		}
	});
	bot->on_slashcommand([this,bot](const auto& event) {
		try {
			//uint64_t retId = std::get<uint64_t>(this->testCon->getUser((uint64_t)(event.command.get_issuing_user().id))[0]);
			//uint64_t retIdT = std::get<uint64_t>(this->testCon->getUser(((uint64_t)(event.command.get_issuing_user().id)),((uint64_t)(event.command.get_guild().id)))[0]);
			//std::cout << std::endl << "Cur user is: " << std::to_string(retId) << " and: " << retIdT << std::endl << std::endl;
			std::thread([this,event,bot] { this->slashCmds.at(event.command.get_command_name())(event); }).detach();
		} catch (...) {
			std::cout << "An error has occured" << std::endl;
		}
	});
	
	

	this->addSlashCmd("joinqueue",[bot,this](const slashcommand_t &event) {
		event.thinking(true);
		auto subcommand = event.command.get_command_interaction().options[0];
		int gameInt = scopeNums[subcommand.name];
		int scopeInt = subcommand.get_value<int64_t>(0);
		(new std::thread([=,this] {
			if (this->curQueues[gameInt][scopeInt]->addPlayerInt(snowPair(event.command.usr.id,((bot->thread_create_sync("testT",event.command.channel_id,1440,CHANNEL_PRIVATE_THREAD,true,1))).id))) {
				event.edit_response("Queue joined succesfully, please standby");
			} else {
			
				event.edit_response(":x: There has been a error joining the queue");
			}
		}))->detach();
	});
	this->addSlashCmd("info",[bot](const slashcommand_t &event) {
		event.reply("This bot is for playing two player games, and it's source code may be found at https://github.com/wizard7377/duelBot.git");
	});
	this->addSlashCmd("challenge",[this,bot](const slashcommand_t &event) {
		//std::cout << "started\n";
		if (checkExists(event.command.usr.id,std::get<dpp::snowflake>(event.get_parameter("player")))) {
			event.reply(message("You have already challenged, or are already being challenged by this player!").set_flags(m_ephemeral));
			return;
		}
		//command_interaction cmdData = std::get<command_interaction>(event.command.data);
		interaction_modal_response gameForm(("userForm"+event.command.usr.username),"Enter to create challenge against oponent, enter none to get no time control");
		gameForm.add_component(
			component().
			set_label("Start time (minutes)").
			set_id("startT").
			set_type(cot_text).
			set_placeholder("0").
			set_min_length(0).
			set_max_length(4).
			set_text_style(text_short).
			set_required(false)
		);
		gameForm.add_row();
		gameForm.add_component(
			component().
			set_label("Increment time (seconds)").
			set_id("addT").
			set_type(cot_text).
			set_placeholder("0").
			set_min_length(0).
			set_max_length(4).
			set_text_style(text_short).
			set_required(false)
		);
		gameForm.add_row();
		gameForm.add_component(
			component().
			set_label("delay time (seconds)").
			set_id("delayT").
			set_type(cot_text).
			set_placeholder("0").
			set_min_length(0).
			set_max_length(4).
			set_text_style(text_short).
			set_required(false)
		);
	
		event.dialog(gameForm);
		//std::cout << std::endl << event.get_parameter("player").index() << std::endl;
		//std::cout << std::endl << event.get_parameter("game").index() << std::endl;
		//std::cout << std::endl << std::get<std::string>(event.get_parameter("player")) << std::endl;
		bool isRank;
		
		try { isRank = std::get<bool>(event.get_parameter("ranked")); } catch (...) { isRank = false; }
		
		this->addFormCmd(("userForm"+event.command.usr.username),([event,bot,isRank](const form_submit_t& eventPar) {
			
			handleChallengeSubmit(event.command.get_issuing_user(),std::get<dpp::snowflake>(event.get_parameter("player")),std::get<std::string>(event.get_parameter("game")),event.command.get_guild().name,(*bot),eventPar,isRank);
				
		}));
		
	});
	
	this->addSlashCmd("getrate",[this,bot](const slashcommand_t &event) {
		//std::cout << "cmd recived\n";
		try {
			event.thinking(true);
	
			(new std::thread([this,event] {
				try {

					snowflake searchP = event.command.get_issuing_user().id;
					bool isPlayer = false;
					if (((event.get_parameter("player")).index()) == 0) {
						isPlayer = true;
					} else {
					
						if (std::get<snowflake>(event.get_parameter("player")) == searchP) {
						
							isPlayer = true;
						} else {
			
							searchP = std::get<snowflake>(event.get_parameter("player"));
						}
					}
						

					reSet valSet = (this->testCon->getRate(gameNums[std::get<std::string>(event.get_parameter("game"))],searchP));
			
					int res = std::get<int>(valSet[0]);					
					bool canShow = std::get<bool>(valSet[1]);
					//std::cout << "Can show and isPlayer are: " << std::to_string(canShow)  << ", and " << std::to_string(isPlayer) << std::endl;
					if (canShow or isPlayer) { event.edit_response(("Got: " + std::to_string(res))); }
					else { event.edit_response(":x: You do not have permission to view this player's rating"); };

				
				} catch (...) {
				
				}
			}))->detach();
			
		} catch (...) {
		
			event.edit_response("Bot dun goffed up");
		
		}
	});

	this->addSlashCmd("changesetting",[this,bot](const slashcommand_t &event) {
		event.thinking(true);
		auto cmd_data = event.command.get_command_interaction();
		auto bToStr = ([] (bool inVal) {
			if (inVal) {
				return "1";
			} else {
				return "0";
			}
		});
		auto subCmd = cmd_data.options[0];
		bool runGood = false;
		if (subCmd.name == "showrate") {
			if (this->testCon->editSetting("showRate",bToStr(subCmd.get_value<bool>(0)),gameNums.at(subCmd.get_value<std::string>(1)),event.command.get_issuing_user().id,event.command.get_guild().id,scopeNums.at(subCmd.get_value<std::string>(2)))) {
				event.edit_response("Done!");
			} else {
				event.edit_response("Fail!");

			}
		}
			
			
	});
	
		


		



	std::cout << "Event handelers succesfully started" << std::endl;

}
//bool eventhandle::addSelectCmd(std::string compid,std::function<void(const select_click_t&)> newCmd) {
bool eventhandle::addSelectCmd(std::string compid,std::function<void(const select_click_t&)> newCmd) {
	try { this->selectCmds.emplace(compid,newCmd); }
	catch (...) { return false; }
	return true;
}
bool eventhandle::addSlashCmd(std::string compid,std::function<void(const slashcommand_t&)> newCmd) {
	try { this->slashCmds.emplace(compid,newCmd); }
	catch (...) { return false; }
	return true;
}
bool eventhandle::addFormCmd(std::string compid,std::function<void(const form_submit_t&)> newCmd) {
	try { this->formCmds.emplace(compid,newCmd); }
	catch (...) { return false; }
	return true;
}
bool eventhandle::addButtonCmd(std::string compid,std::function<void(const button_click_t&)> newCmd) {
	try { this->buttonCmds.emplace(compid,newCmd); }
	catch (...) { return false; }
	return true;
}




bool eventhandle::deleteSelectCmd(std::string compid) {
	try { this->selectCmds.erase(compid); }
	catch (...) { return false; }
	return true;
}
bool eventhandle::deleteSlashCmd(std::string compid) {
	try { this->slashCmds.erase(compid); }
	catch (...) { return false; }
	return true;
}
bool eventhandle::deleteFormCmd(std::string compid) {
	try { this->slashCmds.erase(compid); }
	catch (...) { return false; }
	return true;
}
bool eventhandle::deleteButtonCmd(std::string compid) {
	try { this->buttonCmds.erase(compid); }
	catch (...) { return false; }
	return true;
}


}
