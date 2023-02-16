#include <thread>
#include <string>
#include <dpp/dpp.h>
#include <functional>
#include <unordered_map>
#include <iostream>
#include "gamenums.hpp"
#include "eventhandle.hpp"

extern void handleChallengeSubmit(user userId, snowflake challengeId, std::string gameName, std::string guildName, cluster& bot,const form_submit_t& event);

using namespace dpp;

namespace evt {
	
eventhandle::eventhandle(cluster * bot) {
	this->testCon = new mData::dataHandle();
	
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

	this->addSlashCmd("info",[bot](const slashcommand_t &event) {
		event.reply("This bot is for playing two player games, and it's source code may be found at https://github.com/wizard7377/duelBot.git");
	});
	this->addSlashCmd("challenge",[this,bot](const slashcommand_t &event) {
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
		this->addFormCmd(("userForm"+event.command.usr.username),([event,bot](const form_submit_t& eventPar) {	
			handleChallengeSubmit(event.command.get_issuing_user(),std::get<dpp::snowflake>(event.get_parameter("player")),std::get<std::string>(event.get_parameter("game")),event.command.get_guild().name,(*bot),eventPar);
		}));
	});
	
	this->addSlashCmd("getrate",[this,bot](const slashcommand_t &event) {
		std::cout << "cmd recived\n";
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
					std::cout << "Can show and isPlayer are: " << std::to_string(canShow)  << ", and " << std::to_string(isPlayer) << std::endl;
					if (canShow or isPlayer) { event.edit_response(("Got: " + std::to_string(res))); }
					else { event.edit_response(":x: You do not have permission to view this player's rating"); };

				
				} catch (...) {
				
				}
			}))->detach();
			
		} catch (...) {
		
			event.edit_response("Bot dun goffed up");
		
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
