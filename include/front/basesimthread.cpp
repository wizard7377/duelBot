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
#include "utl.hpp"
#include <cstdlib>
#include <random>
#include <thread>
#include "eventhandle.hpp"
#include <chrono>
#include <cmath>
using namespace dpp;

//template class gameInt::baseGameInt<game::baseGameLogic>;

template class gameFront::baseSimThread<game::ticTacToeLogic>;
template class gameFront::baseSimThread<game::checkersLogic>;
template class gameFront::baseSimThread<game::baseGameLogic>;



std::string gToStr(gameTime inTime) {
	int numSec = std::chrono::duration_cast<std::chrono::seconds>(inTime).count();
	std::string curStr = "";
	if (numSec >= 60) {
		curStr = curStr + std::to_string((int)(numSec/60)) + ":";
	} 
	if ((int)(numSec%60) >= 10) {
		curStr = curStr + std::to_string((int)(numSec%60));
	} else {
		curStr = curStr + "0" + std::to_string((int)(numSec%60));
	}
	return curStr;
}



namespace gameFront {

/*
template <typename T>
baseSimThread<T>::~baseSimThread() {
	this->bot->on_button_click.detach(this->buttonEventId);
	this->bot->on_select_click.detach(this->selectEventId);
}
*/

template <typename T>
baseSimThread<T>::baseSimThread(cluster* botPar, snowflake userIdA, snowflake userIdB, snowflake threadId, evt::eventhandle * handlerPar,gameInt::baseGameInt<T> * shareInt) {
	//std::cout << "Wizard goffy (We\'re doing this again... seriously?) 1" << std::endl;
	this->bot = botPar;
	this->userIdOne = userIdA;
	this->userIdTwo = userIdB;
	this->gameThread = threadId;	
	this->emojiCode = gameCon.at(std::type_index(typeid(T))).gameId;
	this->gameDraw = new dg::basicDrawGame(gameCon.at(std::type_index(typeid(T))).gameName);
	this->handler = handlerPar;
	this->gameInteraction = shareInt;
	//mas lazy
	this->isPlayerOne = (userIdA >= userIdB);
	
	
	
	
	

	

	gameInt::gameTimeType* con[] = {new gameInt::gameTimeType(0,0,0),new gameInt::gameTimeType(0,0,0),new gameInt::gameTimeType(0,0,0)};
	//this->gameInteraction = new gameInt::baseGameInt<T>(con,(std::bind(&baseSimThread::endCall,this,std::placeholders::_1,std::placeholders::_2)));
	//std::cout << this->gameThread << std::endl;

	if (this->isPlayerOne) {
		this->msgMake();
	} else {
		this->bot->message_create(message(this->gameThread,"You don't go first"));
	}

	

	
	//do more config stuff later
	//+ stuff with sync
	//+make into its own function
	//+make better comments
	
		

	//std::cout << "finished" << std::endl;
	
	
	
}


template <typename T>
void baseSimThread<T>::endCall(bool userWon, int winCase) {
	message * msg;
	if (userWon) {
		msg = new message((this->gameThread),winMsgs.at(winCase).first);
	} else {
		msg = new message((this->gameThread),winMsgs.at(winCase).second);
	}
	this->bot->message_create(*msg);
}




template <typename T>
message * baseSimThread<T>::msgMake() {
	message * msg = makeGameEmbed();

	std::vector<std::string> inMoves;
	if (this->gameInteraction->isDuoMove()) {
		for (int i = 0; i < this->gameInteraction->getAllMoves().size(); i++) {
			//std::cout << this->gameInteraction->getAllMoves()[i].size();
			if (this->gameInteraction->getAllMoves()[i].size() != 0) { inMoves.push_back(this->gameInteraction->intToMove(i)); }
		}
	} else {
		for (int i = 0; i < this->gameInteraction->getAllMoves().size(); i++) {
			//std::cout << this->gameInteraction->getAllMoves()[i].size();
			if (this->gameInteraction->getAllMoves()[i].size() != 0) { inMoves.push_back(this->gameInteraction->getAllMoves()[i][0]); }
		}
	}
	//std::cout <<  __LINE__ << std::endl;
	std::string ranPre = std::to_string(rand());
	std::string itemIds[] = {reqId(),reqId(),reqId(),reqId(),reqId(),reqId(),reqId(),reqId(),reqId(),reqId()};
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
	if (this->gameInteraction->isDuoMove()) {
		msg->add_component(
			component().add_component(
				endSel->pageStay()
			)
		);
	}
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
	msg->add_component(
	component().add_component(
		component().set_label("Request draw").
		set_type(cot_button).
		set_style(cos_secondary).
		set_id(itemIds[6])   
	).add_component(
		component().set_label("resign").
		set_type(cot_button).
		set_style(cos_danger).
		set_id(itemIds[7])
	)
	.add_component(
		component().set_label("make move").
		set_type(cot_button).
		set_style(cos_success).
		set_id(itemIds[8])   
	)
	);

	
	if (this->imgThread->joinable()) { this->imgThread->join(); }
	//std::cout << std::to_string(msg->id) << std::endl;

	this->bot->message_create(*msg,[msg](const confirmation_callback_t & event) {
		*msg = std::get<message>(event.value);
		//std::cout << msg->id << std::endl;
	});


	
	//std::cout << std::to_string(msg->id) << std::endl;
	

	
	this->handler->addSelectCmd(msg->components[0].components[0].custom_id,[endSel,msg,this](const select_click_t & event) mutable {
	
		if (this->gameInteraction->isDuoMove()) {
			this->handler->deleteSelectCmd(msg->components[2].components[0].custom_id);
			delete endSel;
			endSel = new (utl::bigSelect)(this->gameInteraction->getAllMoves()[this->gameInteraction->moveToInt(event.values[0])]);			
			msg->components[2].components[0] = endSel->pageStay();
			this->handler->addSelectCmd(msg->components[2].components[0].custom_id,[endSel,msg,this](const select_click_t & event) mutable {	
				msg->components[2].components[0].set_placeholder(event.values[0]);
				event.reply();
				this->curMove[1] = event.values[0];	
			});
		}
		msg->components[0].components[0].set_placeholder(event.values[0]);
		event.reply();
		this->curMove[0] = event.values[0];
		this->curMove[1] = "";
		this->bot->message_edit(*msg);
	});
	if (this->gameInteraction->isDuoMove()) {	
		this->handler->addSelectCmd(msg->components[2].components[0].custom_id,[endSel,msg,this](const select_click_t & event) mutable {	
			msg->components[2].components[0].set_placeholder(event.values[0]);
			event.reply();
			this->curMove[1] = event.values[0];	
		});
	}
	
		
	this->handler->addButtonCmd(itemIds[8], [itemIds,this](const auto& event) {
		if ((this->curMove[0] == "") or ((this->curMove[1] == "") and (this->gameInteraction->isDuoMove()))) {
			event.reply(message(":x: You haven't selected a move yet!").set_flags(m_ephemeral));
		} else {
			event.thinking();
			this->gameInteraction->makeMove(this->curPlayer,curMove[0],curMove[1]);
			//event.reply(this->makeGameEmbed());
			message * nMsg = this->makeGameEmbed();
			nMsg->set_content("Move made!");
			try { this->imgThread->join(); } catch (...) {}
			event.edit_original_response(*nMsg);
			this->handler->deleteButtonCmd(itemIds[8]);	
			message eMsg = event.command.msg;
			eMsg.components = std::vector<component>();
			eMsg.filecontent = event.command.msg.filecontent;
			eMsg.filename = event.command.msg.filename;
			this->bot->message_edit(eMsg);
			//this->msgMake();
			this->giveMove();
		}
			
	});
	this->handler->addButtonCmd(itemIds[0],[startSel,msg,this](const button_click_t& event) {
		msg->components[0].components[0] = startSel->pageUp();
		this->bot->message_edit(*msg);
	});
	//std::cout << "Wait a minute..." << std::endl;	
	this->handler->addButtonCmd(itemIds[2],[startSel,msg,this](const button_click_t& event) {
		msg->components[0].components[0] = startSel->pageDown();
		this->bot->message_edit(*msg);
	});
	if (this->gameInteraction->isDuoMove()) {
		this->handler->addButtonCmd(itemIds[3],[endSel,msg,this](const button_click_t& event) {
			msg->components[2].components[0] = endSel->pageUp();
			this->bot->message_edit(*msg);
		});
		this->handler->addButtonCmd(itemIds[5],[endSel,msg,this](const button_click_t& event) {
			msg->components[2].components[0] = endSel->pageDown();
			this->bot->message_edit(*msg);
		});
	}
	return msg;

}

template <typename T>
message * baseSimThread<T>::makeGameEmbed() {
	
	std::string imgPath = this->gameDraw->getBoard(this->gameInteraction->getBoard());
	
	embed mainEmb = embed().
		set_color(colors::blue_aquamarine).
		set_title("Move: of the game between: " + bot->user_get_sync(this->userIdOne).username + " and " + bot->user_get_sync(userIdTwo).username).
		set_author("Duel Bot","https://github.com/wizard7377/duelBot.git",(bot->current_user_get_sync().get_avatar_url())).
		set_image("attachment://game.png").
		add_field("Time left (you)", gToStr(this->gameInteraction->timeMove(this->isPlayerOne))).
		add_field("Time left (opponent)", gToStr(this->gameInteraction->timeMove(!this->isPlayerOne)));

	message * msg = new message((this->gameThread),mainEmb);
	//std::cout << this->isPlayerOne << "-" << gToStr(this->gameInteraction->timeMove(this->isPlayerOne)) << "  AND  " << !this->isPlayerOne << "-" << gToStr(this->gameInteraction->timeMove(!this->isPlayerOne)) << std::endl;
	std::cout << this->isPlayerOne << "  AND  " << !this->isPlayerOne << std::endl;
	this->imgThread = new std::thread( [this,msg,imgPath] { msg->add_file("game.png",utility::read_file(imgPath)); } );
	return msg;

}

template <typename T>
void baseSimThread<T>::getMove() {
	this->msgMake();
	//this->giveMove();
}

	









template <typename T>
std::string baseSimThread<T>::drawBoard(bool userMove, std::vector<std::vector<int>> boardState) {
		
	
	
	char i = 'a';
	std::string retString = ":white_large_square:";

	
	for (auto c : boardState) {
	    //retString = retString + charToEmote[i];
		//std::cout << std::to_string((int)(i)) << std::endl;
		i++;
	}
	retString = retString + '\n';
	

	
	i = '1';

	for (auto a : boardState) {
		//retString = retString+ charToEmote[i];
		for (int b : a) {
			retString= retString+ "<:" + (this->emojiCode)+"000"+addChar(std::to_string(b))+":"+gamesToEmojis[this->emojiCode]["000"][b] + ">";
		
		}
		i++;
		retString = retString + '\n';
	}
	
	//std::cout << retString;
	return retString;

		
}



}
