#include "frontend.hpp"
#include <format>
#include <dpp/dpp.h>
#include <concepts>
#include <functional>
#include <spdlog/spdlog.h>
#include "gameInteraction.hpp"
#include "rateQueue.hpp"
#include "colorUtil.hpp"
//TODO UNDO
template class gameFront::baseGameHandle<game::ticTacToeLogic>;

template class gameFront::baseGameHandle<game::checkersLogic>;
/*
template class gameFront::baseGameHandle<game::chessLogic>;
template class gameFront::baseGameHandle<game::connectLogic>;
*/
//template class gameFront::baseGameHandle<game::baseGameLogic>;

using namespace dpp;

static int64_t numGames = 0;
namespace gameFront {

template <typename T>
baseGameHandle<T>::baseGameHandle(std::function<inType*(gameInt::baseGameInt<T>*)> inHandles[2],std::vector<gameTime> inTimes) {
	//std::cout << colorForm("Started",GREEN_TERM) << std::endl;
	numGames++;
	spdlog::info("The current number of games is: {}", numGames);
	gameTime * tCon[3] = {new gameTime(0),new gameTime(0),new gameTime(0)};
	
	//std::cout << "At line: " << colorForm(std::to_string(__LINE__),RED_TERM) << std::endl;
	
	

	const std::function<void(bool,int)> iEnd = ([this](bool winner, int winCase) {
		if (!(this->ended)) {
			numGames--;
			spdlog::info("The current number of games is: {}\n",colorForm(std::to_string(numGames),RED_TERM));
			bool isTie = (winCase < 0);
			setRates(this->gameHandles[0]->handler->testCon,this->gameHandles[0]->userIdOne,this->gameHandles[0]->userIdTwo,0,winner,this->gameHandles[0]->guildId,this->gameHandles[1]->guildId,isTie); // weeeeeeeeeee very lazy
			//setRates(this->gameHandles[1]->handler->testCon,this->gameHandles[1]->userIdOne,this->gameHandles[1]->userIdTwo,0,!winner,isTie); // weeeeeeeeeee very lazy
			this->gameHandles[0]->endCall(winner,winCase);
			this->gameHandles[1]->endCall(!winner,winCase);
			this->ended = true;
			
			//delete this->gameHandles[0];
			//delete this->gameHandles[1];
		}
	}); 
	//int setRates(mData::dataHandle * dataCon,snowflake setPlayer, snowflake otherPlayer, int gameVal, bool isWinner = true, snowflake guildVal = 0,snowflake otherGuild = 0);
	this->shState = new gameInt::baseGameInt<T>(inTimes.data(),iEnd);
	for (int i = 0; i <= 1; i++) {
		//std::cout << "At line: " << colorForm(std::to_string(__LINE__),RED_TERM) << std::endl;
		this->gameHandles[i] = (baseSimThread<T>*)(inHandles[i](this->shState));
		//std::cout << "At line: " << colorForm(std::to_string(__LINE__),RED_TERM) << std::endl;
	}
	
	//std::cout << "At line: " << colorForm(std::to_string(__LINE__),RED_TERM) << std::endl;
	this->gameHandles[0]->giveMove = ( [this] {this->gameHandles[1]->getMove(); } );
	this->gameHandles[1]->giveMove = ( [this] {this->gameHandles[0]->getMove(); } );
	this->gameHandles[0]->drawCall = ( [this] {this->gameHandles[1]->requestDraw(); } );
	this->gameHandles[1]->drawCall = ( [this] {this->gameHandles[0]->requestDraw(); } );



}

template <typename T>
void baseGameHandle<T>::requestDraw(bool inPlayerOne) {
	int playerNum = (int)(!inPlayerOne);
	//std::cout << "Draw requested\n";
	this->gameHandles[playerNum]->requestDraw();
}


}

