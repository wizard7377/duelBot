#pragma once
#include <dpp/dpp.h>
#include <string>
#include <map>
#include "eventHandler.hpp"

using namespace dpp;


extern void handleChallengeSubmit(user userId, snowflake challengeId, std::string gameName, std::string guildName, cluster& bot,const form_submit_t& event);
extern evt::eventhandle * handler;

command_option getScopeChoice() {
	return (command_option(co_string,"effectglobal","What should this command effect (defaults to only this)",false)
		.add_choice(command_option_choice("affect only this server","this"))
		.add_choice(command_option_choice("affect only global setting","global"))
		.add_choice(command_option_choice("affect both","both"))
		.add_choice(command_option_choice("affect all servers and global","all"))
		.add_choice(command_option_choice("affect all servers, but not global","allbut"))
	);
}

namespace botCmds {
//TODO Make command creation cleaner
/*! Info command */
dpp::slashcommand infoDef() {
	return dpp::slashcommand()
		.set_name("info")
		.set_description("info on the bot");
}
/*! Challenge command */
dpp::slashcommand challengeDef() {
	return dpp::slashcommand().set_name("challenge").set_description("challenge user to a game")
	.add_option(
		dpp::command_option(dpp::co_string, "game", "game you wish to play", true).
			add_choice(dpp::command_option_choice("tictactoe", std::string("tictactoe"))).
			add_choice(dpp::command_option_choice("Connect 4", std::string("connectfour"))).
			add_choice(dpp::command_option_choice("checkers", std::string("checkers"))).
			add_choice(dpp::command_option_choice("go (game)", std::string("goboardgame"))).
			add_choice(dpp::command_option_choice("chess", std::string("chess")))
	)
    .add_option(
		dpp::command_option(dpp::co_user, "player", "player you will be facting in your epic duel",true)
	)
	.add_option(command_option(co_boolean,"ranked","Should the challenge be of a ranked game?",false));
}
/*! Get rating command */
dpp::slashcommand getRateDef() {
	return (dpp::slashcommand().set_name("getRate").set_description("Get the rating of a user")
	.add_option(command_option(co_string, "game", "game you wish to get rating of", true).
			add_choice(dpp::command_option_choice("tictactoe", std::string("tictactoe"))).
			add_choice(dpp::command_option_choice("Connect 4", std::string("connectfour"))).
			add_choice(dpp::command_option_choice("checkers", std::string("checkers"))).
			add_choice(dpp::command_option_choice("go (game)", std::string("goboardgame"))).
			add_choice(dpp::command_option_choice("chess", std::string("chess"))))
	.add_option(command_option(co_user, "player", "player you wish to get the rating of (leave blank for self)",false))
	);
}

/*! Change setting command */
slashcommand changeSetDef() {
	return (slashcommand().set_name("changeSetting").set_description("Change vaue of a setting")

	.add_option(command_option(co_sub_command,"showrate","allow others to see your rating")
		.add_option(command_option(co_boolean,"value","Should the players in this guild be able to see your rating?",true))
		.add_option(command_option(co_string,"gametype","Game type",false)
			.add_choice(command_option_choice("TicTacToe","tictactoe"))
			.add_choice(command_option_choice("Checkers","checkers"))
		)
		.add_option(getScopeChoice())
	)
	);
		
}
/*! Join queue command */
slashcommand joinQueueDef() {
	command_option defTimeOps = command_option(co_integer,"timecon","what time control will u use",false)
			.add_choice(command_option_choice("5 | 5",0))
			.add_choice(command_option_choice("5 | 0",1))
			//TEST TEMP lose in less than two minute
			.add_choice(command_option_choice("TEST",2));
	return (slashcommand().set_name("joinQueue").set_description("Join a queue for a game")

	.add_option(command_option(co_sub_command,"tictactoe","Play a game of tictactoe against someone else").add_option(defTimeOps))
		
	.add_option(command_option(co_sub_command,"connectfour","Play a game of Connect 4 against someone else").add_option(defTimeOps))
	.add_option(command_option(co_sub_command,"checkers","Play a game of checkers against someone else").add_option(defTimeOps))
		
	.add_option(command_option(co_sub_command,"chess","Play a game of chess against someone else").add_option(defTimeOps))
	);
		
		
	
}


/*
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
*/


}
