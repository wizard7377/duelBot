#include <dpp/dpp.h>
#include <map>
#include <config.hpp>
#include "gameLogic.hpp"
#include "commandHandle.hpp"
#include <iostream>
#include <functional>
using namespace dpp;

std::map<std::string,std::function<void(cluster&,const slashcommand_t&)>> slashCmds;
std::map<std::string,std::function<void(cluster&,const form_submit_t&)>> formCmds;

void handleChallengeSubmit(std::string userId, snowflake challengeId, std::string gameName, std::string guildName, cluster& bot,const form_submit_t& event) {
	std::string msg = "Hello, the user @" + userId + " has challenged you to a game of " + gameName + " in " + guildName + " do you accept?";
	bot.direct_message_create(challengeId,msg);
}

void createCommandHandle() {
	slashCmds.emplace("info",([](cluster &bot,const slashcommand_t &event) {
		event.reply("This bot is for playing two player games, and it's source code may be found at https://github.com/wizard7377/duelBot.git");
	}));


	slashCmds.emplace("challenge",([](cluster &bot,const slashcommand_t &event) {
		command_interaction cmdData = std::get<command_interaction>(event.command.data);
		interaction_modal_response gameForm(("userForm"+event.command.usr.username),"Enter to create challenge against oponent");
		gameForm.add_component(
			component().
			set_label("Start time").
			set_id("startT").
			set_type(cot_text).
			set_placeholder("0:0:0").
			set_min_length(0).
			set_max_length(10).
			set_text_style(text_short).
			set_required(true)
		);
		gameForm.add_row();
		gameForm.add_component(
			component().
			set_label("Increment time").
			set_id("addT").
			set_type(cot_text).
			set_placeholder("0:0:0").
			set_min_length(0).
			set_max_length(10).
			set_text_style(text_short).
			set_required(false)
		);
		gameForm.add_row();
		gameForm.add_component(
			component().
			set_label("delay time").
			set_id("delayT").
			set_type(cot_text).
			set_placeholder("0:0:0").
			set_min_length(0).
			set_max_length(10).
			set_text_style(text_short).
			set_required(false)
		);
		event.dialog(gameForm);
		formCmds.emplace(("userForm"+event.command.usr.username),([&](cluster& botPar, const form_submit_t& eventPar) {
			handleChallengeSubmit(event.command.usr.username,std::get<snowflake>(event.get_parameter("user")),std::get<std::string>(event.get_parameter("game")),"lazy place",botPar,eventPar);
		}));


		

	}));	
}

int main(int argc, char *argv[]) {

	createCommandHandle();
	
	cluster bot(BOT_TOKEN);
	bot.on_log(utility::cout_logger());

	if (argc != 0) {
		if (std::string(argv[0]) == "1") {
			
		}
	}
	bot.on_slashcommand([&bot](const slashcommand_t &event) {
		(slashCmds.at(event.command.get_command_name()))(bot,event);
	});
	


	bot.on_ready([&bot](const ready_t & event) {
        if (run_once<struct register_bot_commands>()) {
			
			bot.global_command_create(botCmds::infoDef());
			bot.global_command_create(botCmds::challengeDef());
	}
    });
	

	bot.start(st_wait);

	



}
