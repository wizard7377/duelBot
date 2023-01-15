#include <dpp/dpp.h>
#include <map>
#include <variant>
#include <cctype>
#include <config.hpp>
#include "gameLogic.hpp"
#include "commandHandle.hpp"
#include "baseGameInt.hpp"
#include "frontend.hpp"
#include <iostream>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include <cmath>
using namespace dpp;



using snowPair = std::pair<snowflake,snowflake>;

std::map<std::string,std::function<void(cluster&,const slashcommand_t&)>> slashCmds;
std::map<std::string,std::function<void(cluster&,const form_submit_t&)>> formCmds;
std::map<std::string,std::function<void(cluster&,const button_click_t&)>> buttonCmds;
std::map<snowPair,snowflake*> userThreads;
std::map<snowflake,gameFront::wrapThread*> gameObjs;


std::string getTimePar(int index,const form_submit_t& event) {
std::string inString = std::get<std::string>(event.components[index].components[0].value);

	for (char c : inString) {
		if (!std::isdigit(c)) {
			return "";
		}
	}
	return inString;
	
}




bool threadOnChallenge(cluster& bot, snowflake userOne, snowflake userTwo, snowflake channelId) {
	bot.thread_create("testT",channelId,1440,CHANNEL_PRIVATE_THREAD,true,1,[userOne,userTwo,&bot,&userThreads](const confirmation_callback_t& event) {
			thread threadObj = std::get<thread>(event.value);
			snowflake * threadId = new snowflake(threadObj.id);
			snowflake tempArr[2] = {std::min(userOne,userTwo),std::max(userOne,userTwo)};
			userThreads.emplace(std::make_pair(tempArr[0],tempArr[1]),threadId);
	});
	return true;
}

snowflake threadDecide(cluster& bot,snowflake userOne,snowflake userTwo,bool yesOrNo = true) {
	snowPair tempArr = std::make_pair(std::min(userOne,userTwo),std::max(userOne,userTwo));


			
	snowflake threadId = *(userThreads.at(tempArr));
	bot.channel_get(threadId,[yesOrNo,threadId,userOne,userTwo,&bot](const confirmation_callback_t& event) {
		channel threadObj = std::get<channel>(event.value);
		if (yesOrNo) {
			bot.thread_member_add(threadId,userOne);
			bot.thread_member_add(threadId,userTwo);		
			message startMessage(threadId,"Challenge accepted!");
			bot.message_create(startMessage);
		} else {
			bot.channel_delete(threadId);
		}
	});
	userThreads.erase(tempArr);
	return threadId;
}

std::map<std::string,std::function<gameFront::wrapThread*(cluster &bot,user userId,snowflake challengeId,snowflake res)>> typeCmds = {
	{
	"tictactoe",
	([](cluster &bot,user userId,snowflake challengeId,snowflake res) {
		
		return new gameFront::baseThread<game::ticTacToeLogic>(&bot,userId.id,challengeId,res,"tictactoe");
		
	})
	},
	{
	"checkers",
	([](cluster &bot,user userId,snowflake challengeId,snowflake res) {
		
		return new gameFront::baseThread<game::checkersLogic>(&bot,userId.id,challengeId,res,"checkers");
		
	})
	}

};
		
			
			
			




	

void handleChallengeSubmit(user userId, snowflake challengeId, std::string gameName, std::string guildName, cluster& bot,const form_submit_t& event) {
	
	std::string timeControls[3] = { getTimePar(0,event),getTimePar(1,event),getTimePar(2,event) };
	message msg;

	if ((timeControls[0]=="")&&(timeControls[1]=="")&&(timeControls[2]=="")) {
		msg = "Hello, the user " + userId.get_mention() + " has challenged you to a game of " + gameName + " with no time controls in " + guildName + ", do you accept?";
	
	} else {
		msg = "Hello, the user " + userId.get_mention() + " has challenged you to a game of " + gameName + " with time controls of " + 
		timeControls[0]+"|"+timeControls[1]+"|"+timeControls[2] + " in " + guildName + ", do you accept?";
	}
	msg.add_component(component().add_component(
			component().set_label("Yes").
			set_type(cot_button).
			set_style(cos_success).
			set_id(std::to_string(userId.id)+std::to_string(challengeId)+"y")
		).add_component(
			component().set_label("No").
			set_type(cot_button).
			set_style(cos_danger).
			set_id(std::to_string(userId.id)+std::to_string(challengeId)+"n")
		)
	);

	bot.direct_message_create(challengeId,msg);
	event.reply(message("Message sent").set_flags(m_ephemeral));

	threadOnChallenge(bot, userId.id, challengeId, event.command.channel_id);

	buttonCmds.emplace(std::to_string(userId.id)+std::to_string(challengeId)+"n", 
	[event,challengeId,userId,&buttonCmds](cluster& botPar,const button_click_t& eventPar) {
		//std::cout << "response one" << std::endl;
		event.reply("Your request has been denied");
		eventPar.reply("You choose to not accept");
		threadDecide(botPar, userId.id, challengeId,false);
		buttonCmds.erase(std::to_string(userId.id)+std::to_string(challengeId)+"n");
		buttonCmds.erase(std::to_string(userId.id)+std::to_string(challengeId)+"y");	
	});
	buttonCmds.emplace(std::to_string(userId.id)+std::to_string(challengeId)+"y", 
	[gameName,event,challengeId,userId,&bot,&buttonCmds](cluster& botPar,const button_click_t& eventPar) {
		event.edit_response("Your request has been accepted");
		eventPar.reply("You choose to accept");
		//tMT(botPar, challengeId, challengeId, event.command.channel_id);
		//gameFront::baseThread<game::baseGameLogic> newThr(&bot,snowflake(userId),challengeId,(makeThread(botPar, snowflake(userId), challengeId, event.command.channel_id)),"tictactoe");
		snowflake res = (threadDecide(botPar, userId.id, challengeId));

		//std::cout << "response two" << std::endl;
		
		((typeCmds[gameName])(bot,userId,challengeId,res));
		buttonCmds.erase(std::to_string(userId.id)+std::to_string(challengeId)+"n");
		buttonCmds.erase(std::to_string(userId.id)+std::to_string(challengeId)+"y");	
	});

	
}

void createCommandHandle() {
	slashCmds.emplace("info", botCmds::infoCmd );


	slashCmds.emplace("challenge",botCmds::challengeCmd);


		

	
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
	
	bot.on_form_submit([&bot](const form_submit_t &event) {
		(formCmds.at(event.custom_id))(bot,event);
	});
	bot.on_button_click([&bot](const button_click_t &event) {

		try {
			
			(buttonCmds.at(event.custom_id))(bot,event);
		} catch (const std::out_of_range& e) {
			std::cout << "Button already clicked: " << e.what() << std::endl;
			event.reply("Already done");
		} catch (const std::exception& e) {
			std::cout << "Button error: " << e.what() << std::endl;
			event.reply("error in process");
		}
	
	});


	bot.on_ready([&bot](const ready_t & event) {
        if (run_once<struct register_bot_commands>()) {
			
			bot.global_command_create(botCmds::infoDef());
			bot.global_command_create(botCmds::challengeDef());
	}
    });
	

	bot.start(st_wait);

	



}
