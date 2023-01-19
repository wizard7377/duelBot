#pragma once
#include <dpp/dpp.h>
#include <string>
#include <map>
#include "eventhandle.hpp"

using namespace dpp;


extern void handleChallengeSubmit(user userId, snowflake challengeId, std::string gameName, std::string guildName, cluster& bot,const form_submit_t& event);
extern evt::eventhandle * handler;



namespace botCmds {


dpp::slashcommand infoDef() {
	return dpp::slashcommand()
		.set_name("info")
		.set_description("info on the bot");
}

dpp::slashcommand challengeDef() {
	return dpp::slashcommand().set_name("challenge").set_description("challenge user to a game")
	.add_option(
		dpp::command_option(dpp::co_string, "game", "game you wish to play", true).
			add_choice(dpp::command_option_choice("tictactoe", std::string("tictactoe"))).
			add_choice(dpp::command_option_choice("checkers", std::string("checkers")))
	)
    .add_option(
		dpp::command_option(dpp::co_user, "player", "player you will be facting in your epic duel",true)
	);


}

std::function<void(const slashcommand_t&,cluster&)> infoCmd = ([](const slashcommand_t &event,cluster& bot) {
		event.reply("This bot is for playing two player games, and it's source code may be found at https://github.com/wizard7377/duelBot.git");
});;

std::function<void(const slashcommand_t&,cluster&)> challengeCmd = ([handler](const slashcommand_t &event,cluster& bot) {
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
		handler->addFormCmd(("userForm"+event.command.usr.username),([event,&bot](const form_submit_t& eventPar) {	
			handleChallengeSubmit(event.command.get_issuing_user(),std::get<dpp::snowflake>(event.get_parameter("player")),std::get<std::string>(event.get_parameter("game")),event.command.get_guild().name,bot,eventPar);
		}));


		

});

}
