#include "frontend.hpp"
#include <functional>
#include <map>
#include <iterator>
#include <string>
#include "baseGameInt.hpp"
#include "gameLogic.hpp"
#include <dpp/dpp.h>
#include <algorithm>
#include "drawgame.hpp"
#include "bigselect.hpp"
#include <cstdlib>
#include <thread>
#include "eventhandle.hpp"
using namespace dpp;

//template class gameInt::baseGameInt<game::baseGameLogic>;

template class gameFront::baseThread<game::ticTacToeLogic>;
template class gameFront::baseThread<game::checkersLogic>;
template class gameFront::baseThread<game::baseGameLogic>;




std::map<std::string,std::string> gameEmojiName = {
    {"tictactoe","000"},
	{"checkers","001"}
};




std::map<std::string,std::map<std::string,std::vector<std::string>>> gamesToEmojis = 
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


std::string addChar(std::string inString) {
	while (inString.length() < 3) {
		inString = "0"+inString;
	}
	return inString;
}

std::map<char,std::string> charToEmote = {
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

namespace gameFront {

/*
template <typename T>
baseThread<T>::~baseThread() {
	this->bot->on_button_click.detach(this->buttonEventId);
	this->bot->on_select_click.detach(this->selectEventId);
}
*/

template <typename T>
baseThread<T>::baseThread(cluster* botPar, snowflake userIdA, snowflake userIdB, snowflake threadId, std::string gameName,evt::eventhandle * handlerPar) {
	//std::cout << "Wizard goffy (We\'re doing this again... seriously?) 1" << std::endl;
	this->bot = botPar;
	this->userIdOne = userIdA;
	this->userIdTwo = userIdB;
	this->gameThread = threadId;
	this->emojiCode = gameEmojiName[gameName];
	this->gameDraw = new dg::basicDrawGame(gameName);
	this->handler = handlerPar;
	
	
	
	

	

	gameInt::gameTimeType* con[] = {new gameInt::gameTimeType(0,0,0),new gameInt::gameTimeType(0,0,0),new gameInt::gameTimeType(0,0,0)};
	this->gameInteraction = new gameInt::baseGameInt<T>(con,(std::bind(&baseThread::endCall,this,std::placeholders::_1,std::placeholders::_2)));
	//std::cout << this->gameThread << std::endl;

	
	std::string imgPath = this->gameDraw->getBoard(this->gameInteraction->getBoard());

	
	//do more config stuff later
	//+ stuff with sync
	//+make into its own function
	//+make better comments
	embed mainEmb = embed().
		set_color(colors::blue_aquamarine).
		set_title("Move: of the game between: " + bot->user_get_sync(userIdA).username + " and " + bot->user_get_sync(userIdA).username).
		set_author("Duel Bot","https://github.com/wizard7377/duelBot.git",(bot->current_user_get_sync().get_avatar_url())).
		set_image("attachment://game.png");

	message * msg = new message((this->gameThread),mainEmb);
	msg->add_file("game.png",utility::read_file(imgPath));

	std::vector<std::string> inMoves;
	for (int i = 0; i < this->gameInteraction->getAllMoves().size(); i++) {
		inMoves.push_back(this->gameInteraction->intToMove(i));
	}
	std::cout <<  __LINE__ << std::endl;
	std::string ranPre = std::to_string(rand());
	std::string itemIds[] = {ranPre+std::to_string(rand()),ranPre+std::to_string(rand()),ranPre+std::to_string(rand()),ranPre+std::to_string(rand()),ranPre+std::to_string(rand()),ranPre+std::to_string(rand())};
	utl::bigSelect * startSel = new utl::bigSelect(inMoves);
	utl::bigSelect * endSel = new utl::bigSelect(this->gameInteraction->getAllMoves()[0]);
	msg->add_component(
		component().add_component(
			startSel->pageStay()
		)
	);
	msg->add_component(
	component().add_component(
		component().set_label("<-").
		set_type(cot_button).
		set_style(cos_secondary).
		set_id(itemIds[0]).   
		set_disabled(!startSel->canPage[0])
	).add_component(
		component().set_label("?/?").
		set_type(cot_button).
		set_style(cos_secondary).
		set_id(itemIds[1]).   
		set_disabled(true)
	)
	.add_component(
		component().set_label("->").
		set_type(cot_button).
		set_style(cos_secondary).
		set_id(itemIds[2]).   
		set_disabled(!startSel->canPage[1])
	)
	);
	msg->add_component(
		component().add_component(
			endSel->pageStay()
		)
	);
	msg->add_component(
	component().add_component(
		component().set_label("<-").
		set_type(cot_button).
		set_style(cos_secondary).
		set_id(itemIds[3]).   
		set_disabled(!endSel->canPage[0])
	).add_component(
		component().set_label("?/?").
		set_type(cot_button).
		set_style(cos_secondary).
		set_id(itemIds[4]).   
		set_disabled(true)
	)
	.add_component(
		component().set_label("->").
		set_type(cot_button).
		set_style(cos_secondary).
		set_id(itemIds[5]).   
		set_disabled(!endSel->canPage[1])
	)
	);

	
	std::cout << std::to_string(msg->id) << std::endl;

	this->bot->message_create(*msg,[&msg](const confirmation_callback_t & event) {
		*msg = std::get<message>(event.value);
		std::cout << msg->id << std::endl;
	});


	
	std::cout << std::to_string(msg->id) << std::endl;
	

	this->handler->addSelectCmd(msg->components[0].components[0].custom_id,[endSel,msg,this](const select_click_t & event) mutable {
		
		delete endSel;
		endSel = new (utl::bigSelect)(this->gameInteraction->getAllMoves()[this->gameInteraction->moveToInt(event.values[0])]);			
		msg->components[2].components[0] = endSel->pageStay();
		event.reply();
		this->bot->message_edit(*msg);
		
	});
	
		
	this->handler->addButtonCmd(itemIds[0],[startSel,msg,this](const button_click_t& event) {
		msg->components[0].components[0] = startSel->pageUp();
		this->bot->message_edit(*msg);
	});
	std::cout << "Wait a minute..." << std::endl;	
	this->handler->addButtonCmd(itemIds[2],[startSel,msg,this](const button_click_t& event) {
		msg->components[0].components[0] = startSel->pageDown();
		this->bot->message_edit(*msg);
	});
	this->handler->addButtonCmd(itemIds[3],[endSel,msg,this](const button_click_t& event) {
		msg->components[2].components[0] = endSel->pageUp();
		this->bot->message_edit(*msg);
	});
	this->handler->addButtonCmd(itemIds[5],[endSel,msg,this](const button_click_t& event) {
		msg->components[2].components[0] = endSel->pageDown();
		this->bot->message_edit(*msg);
	});
		

	std::cout << "finished" << std::endl;
	
	
	
}
				





template <typename T>
std::string baseThread<T>::drawBoard(bool userMove, std::vector<std::vector<int>> boardState) {
		
	
	
	char i = 'a';
	std::string retString = ":white_large_square:";

	
	for (auto c : boardState) {
	    retString = retString + charToEmote[i];
		//std::cout << std::to_string((int)(i)) << std::endl;
		i++;
	}
	retString = retString + '\n';
	

	
	i = '1';

	for (auto a : boardState) {
		retString = retString+ charToEmote[i];
		for (int b : a) {
			retString= retString+ "<:" + (this->emojiCode)+"000"+addChar(std::to_string(b))+":"+gamesToEmojis[this->emojiCode]["000"][b] + ">";
		
		}
		i++;
		retString = retString + '\n';
	}
	
	std::cout << retString;
	return retString;

		
}



}
