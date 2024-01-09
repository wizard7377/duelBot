#pragma once

#include <dpp/dpp.h>
#include "gameInteraction.hpp"
#include "gameLogic.hpp"
#include <typeindex>
#include <typeinfo>
#include <string>
#include <map>
#include <iostream>
#include <atomic>
#include <functional>
#include "drawGame.hpp"
#include "eventHandler.hpp"
#include "frontend.hpp"
#include "frontendUtils.hpp"

/*

extern std::map<std::string,std::string> gameEmojiName;

extern std::map<std::string,std::map<std::string,std::vector<std::string>>> gamesToEmojis;

extern std::map<char,std::string> charToEmote;

*/

using namespace dpp;

namespace gameFront {

class inType {
	public: 
		std::function<void()> giveMove;
		~inType() {};
		virtual void getMove() = 0;  
		virtual void endCall(int userWon, int winType) = 0;  
};
class wrapThread {};
class wrapThreadHandle {};

template <typename T>
class baseSimThread : public wrapThread, public inType {
	public:
	    ~baseSimThread();
	    baseSimThread(cluster* botPar, snowflake userIdA, snowflake userIdB, snowflake threadId, evt::eventhandle * handlerPar,gameInt::baseGameInt<T> * shareInt);
	    //~baseThread();
		static_assert(std::is_base_of<game::baseGameLogic,T>::value, "Base game interactions may only have templates of game types");
		
	    std::function<void(message msg)> moveCall;
	    void endCall(int userWon, int winType);
		void getMove() override;
		std::function<void()> drawCall;

	//private:
		bool timePer = false; //TODO Add setting for time percision, when on, make time in minutes and seconds vs just general
		uint64_t uniId = 1;
	    cluster* bot;
		bool isPlayerOne;
	    snowflake userIdOne;
	    snowflake userIdTwo;
	    snowflake gameThread;
		snowflake guildId = 0; //TODO make this actually something
		evt::eventhandle * handler;
		dpp::thread gameThreadObj;
	    int gameCode;
		dg::baseDrawGame * gameDraw;
	    gameInt::baseGameInt<T> * gameInteraction;
		event_handle buttonEventId;
		event_handle selectEventId;
		message * makeGameEmbed();
		message * msgMake();
		void requestDraw();
		std::thread * imgThread;
		std::string curMove[2] = {"",""};
		std::mutex mtx;
		bool pOneFirst;
		bool curPlayer;
		bool drawReq = true;
		bool isRun = true;
		int currentTurn = 0;
		message * lastMsg;
		
		std::map<std::string,std::function<void(const button_click_t&)>> gameButtonCmds;
		std::map<std::string,std::function<void(const select_click_t&)>> gameSelectCmds;
	private:
		bool imgMade = false;
		std::string imgPath = "";
		std::string getBoard();


};

template <typename T>
class baseGameHandle : public wrapThreadHandle {
	public:
		static_assert(std::is_base_of<game::baseGameLogic,T>::value, "Can only have logic of game types");
		baseGameHandle(std::function<inType*(gameInt::baseGameInt<T>*)> inHandles[2],std::vector<gameTime> inTimes = {10min,5s,6s});
		std::function<void()> onStart;
		gameInt::baseGameInt<T> * shState;

	
		//inType * gameHandles[2];
		baseSimThread<T> * gameHandles[2];
		void requestDraw(bool inPlayerOne);
		bool ended = false;
		





};

/*	
template <typename T>
class baseThread : public wrapThread {
	public:
	    
	    baseThread(cluster* botPar, snowflake userIdA, snowflake userIdB, snowflake threadId, std::string gameName,evt::eventhandle * handlerPar,int rpsTurns = 0);
	    //~baseThread();
		static_assert(std::is_base_of<game::baseGameLogic,T>::value, "Base game interactions may only have templates of game types");
		std::string drawBoard(bool userMove, std::vector<std::vector<int>> boardState);
	    std::function<void(message msg)> moveCall;
	    void endCall(bool userWon, int winType) {
			std::cout << "game has ended :(" << std::endl;
		}

	private:
	    cluster* bot;
	    snowflake userIdOne;
	    snowflake userIdTwo;
	    snowflake gameThread;
		evt::eventhandle * handler;
		channel gameThreadObj;
	    std::string emojiCode;
		dg::baseDrawGame * gameDraw;
	    gameInt::baseGameInt<T> * gameInteraction;
		event_handle buttonEventId;
		event_handle selectEventId;
		message * makeGameEmbed();
		message * msgMake();
		std::thread * imgThread;
		std::string curMove[2] = {"",""};
		std::mutex mtx;
		bool pOneFirst;
		bool curPlayer;
		std::map<std::string,std::function<void(const button_click_t&)>> gameButtonCmds;
		std::map<std::string,std::function<void(const select_click_t&)>> gameSelectCmds;


};
*/





}
