#include <dpp/dpp.h>
#include <map>
#include <variant>
#include "gameLogic.hpp"
//#include "commandHandle.hpp"
#include "gameInteraction.hpp"
#include "databaseLogic.hpp"
#include <iostream>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include "eventHandler.hpp"
#include "frontend.hpp"
#include "gamenums.hpp"
#include "rateQueue.hpp"

using namespace dpp;

using snowPair = std::pair<snowflake,snowflake>;

/*
 * All the command handlers
 */

evt::eventhandle * handler;






std::map<snowPair,snowPair> userThreads;
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
//TODO Undo 
std::map<std::string,std::function<gameFront::wrapThreadHandle*(cluster &bot,user userId,snowflake challengeId,snowPair res,evt::eventhandle * handler)>> typeCmds = {
	{
	"tictactoe",
	([](cluster &bot,user userId,snowflake challengeId,snowPair res,evt::eventhandle * handler) {
		std::function<gameFront::inType*(gameInt::baseGameInt<game::ticTacToeLogic>*)> funcs[2] = {
			([&] (gameInt::baseGameInt<game::ticTacToeLogic>* inState) { return new gameFront::baseSimThread<game::ticTacToeLogic>(&bot,userId.id,challengeId,res.first,handler,inState); })
			,
			([&] (gameInt::baseGameInt<game::ticTacToeLogic>* inState) { return new gameFront::baseSimThread<game::ticTacToeLogic>(&bot,challengeId,userId.id,res.second,handler,inState); })
		};
		return new gameFront::baseGameHandle<game::ticTacToeLogic>(funcs);
		
	})
	},
	
	{
	"checkers",
	([](cluster &bot,user userId,snowflake challengeId,snowPair res,evt::eventhandle * handler) {
		std::function<gameFront::inType*(gameInt::baseGameInt<game::checkersLogic>*)> funcs[2] = {
			([&] (gameInt::baseGameInt<game::checkersLogic>* inState) { return new gameFront::baseSimThread<game::checkersLogic>(&bot,userId.id,challengeId,res.first,handler,inState); })
			,
			([&] (gameInt::baseGameInt<game::checkersLogic>* inState) { return new gameFront::baseSimThread<game::checkersLogic>(&bot,challengeId,userId.id,res.second,handler,inState); })
		};
		return new gameFront::baseGameHandle<game::checkersLogic>(funcs);
		
	})
	}/*
	{
	"connectfour",
	([](cluster &bot,user userId,snowflake challengeId,snowPair res,evt::eventhandle * handler) {
		std::function<gameFront::inType*(gameInt::baseGameInt<game::connectLogic>*)> funcs[2] = {
			([&] (gameInt::baseGameInt<game::connectLogic>* inState) { return new gameFront::baseSimThread<game::connectLogic>(&bot,userId.id,challengeId,res.first,handler,inState); })
			,
			([&] (gameInt::baseGameInt<game::connectLogic>* inState) { return new gameFront::baseSimThread<game::connectLogic>(&bot,challengeId,userId.id,res.second,handler,inState); })
		};
		return new gameFront::baseGameHandle<game::connectLogic>(funcs);
		
	})
	}*/
	

};

bool checkExists(snowflake userOne, snowflake userTwo) {
	snowPair tempArr = {std::min(userOne,userTwo),std::max(userOne,userTwo)};
	return userThreads.contains(tempArr);
}
bool threadOnChallenge(cluster& bot, snowflake userOne, snowflake userTwo, snowflake channelId) {

	std::thread( ([=,&bot]() {
			snowflake tempArr[2] = {std::min(userOne,userTwo),std::max(userOne,userTwo)};
			dpp::user userArr[2] = {*dpp::find_user(userOne),*dpp::find_user(userTwo)};
			snowflake snowOne = (bot.thread_create_sync(std::format("{} vs {}",userArr[0].username,userArr[1].username),channelId,1440,CHANNEL_PRIVATE_THREAD,true,1)).id;
			snowflake snowTwo = (bot.thread_create_sync(std::format("{} vs {}",userArr[1].username,userArr[0].username),channelId,1440,CHANNEL_PRIVATE_THREAD,true,1)).id;
			userThreads.emplace(std::make_pair(tempArr[0],tempArr[1]),std::make_pair(snowOne,snowTwo));
	})).detach();

	return true;
}

snowPair threadDecide(cluster& bot,snowflake userOne,snowflake userTwo,bool yesOrNo = true) {
	
	snowPair tempArr = std::make_pair(std::min(userOne,userTwo),std::max(userOne,userTwo));


			
	snowPair threadId = userThreads.at(tempArr);
	bot.channel_get(threadId.first,[yesOrNo,threadId,userOne,userTwo,&bot](const confirmation_callback_t& event) {
		channel threadObj = std::get<channel>(event.value);
		if (yesOrNo) {
			bot.thread_member_add(threadId.first,userOne);
			//bot.thread_member_add(threadId.first,userTwo);		
			message startMessage(threadId.first,"Challenge accepted!");
			bot.message_create(startMessage);
		} else {
			bot.channel_delete(threadId.first);
		}
	});
	bot.channel_get(threadId.second,[yesOrNo,threadId,userOne,userTwo,&bot](const confirmation_callback_t& event) {
		channel threadObj = std::get<channel>(event.value);
		if (yesOrNo) {
			//bot.thread_member_add(threadId,userOne);
			bot.thread_member_add(threadId.second,userTwo);		
			message startMessage(threadId.second,"Challenge accepted!");
			bot.message_create(startMessage);
		} else {
			bot.channel_delete(threadId.second);
		}
	});
	userThreads.erase(tempArr);
	return threadId;
}

void handleChallengeSubmit(user userId, snowflake challengeId, std::string gameName, std::string guildName, cluster& bot,const form_submit_t& event,bool isRanked = false) {
	
	std::string timeControls[3] = { getTimePar(0,event),getTimePar(1,event),getTimePar(2,event) };
	message msg;
    //std::cout << isRanked << std::endl;
    //std::cout << std::to_string(getRate(handler->testCon,userId.id,gameNums.at(gameName))) << std::endl;
    std::string userMention = userId.get_mention();
	if (isRanked) {
        userMention = userMention + " (" + std::to_string(getRate(handler->testCon,userId.id,gameNums.at(gameName))) + ")";
    }

	if ((timeControls[0]=="")&&(timeControls[1]=="")&&(timeControls[2]=="")) {
		msg = "Hello, the user " + userMention + " has challenged you to a game of " + gameName + " with no time controls in " + guildName + ", do you accept?";
	
	} else {
		msg = "Hello, the user " + userMention + " has challenged you to a game of " + gameName + " with time controls of " + 
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

	handler->addButtonCmd(std::to_string(userId.id)+std::to_string(challengeId)+"n", 
	[event,challengeId,userId,&bot](const button_click_t& eventPar) {
		//std::cout << "response one" << std::endl;
		event.reply("Your request has been denied");
		eventPar.reply("You choose to not accept");
		threadDecide(bot, userId.id, challengeId,false);
		//buttonCmds.erase(std::to_string(userId.id)+std::to_string(challengeId)+"n");
		//buttonCmds.erase(std::to_string(userId.id)+std::to_string(challengeId)+"y");	
	});
	handler->addButtonCmd(std::to_string(userId.id)+std::to_string(challengeId)+"y", 
	[gameName,event,challengeId,userId,&bot](const button_click_t& eventPar) {
		//std::cout << "response two" << std::endl;
		event.edit_response("Your request has been accepted");
		eventPar.reply("You choose to accept");
		//tMT(botPar, challengeId, challengeId, event.command.channel_id);
		//gameFront::baseThread<game::baseGameLogic> newThr(&bot,snowflake(userId),challengeId,(makeThread(botPar, snowflake(userId), challengeId, event.command.channel_id)),"tictactoe");
		snowPair res = (threadDecide(bot, userId.id, challengeId));

		
		
		((typeCmds[gameName])(bot,userId,challengeId,res,handler));
		//buttonCmds.erase(std::to_string(userId.id)+std::to_string(challengeId)+"n");
		//buttonCmds.erase(std::to_string(userId.id)+std::to_string(challengeId)+"y");	
	});

	
}

