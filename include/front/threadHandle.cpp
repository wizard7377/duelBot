#include "frontend.hpp"
#include <dpp/dpp.h>
#include <concepts>
#include <functional>
#include "baseGameInt.hpp"
#include "ratesys.hpp"
#include "colorstuff.hpp"

template class gameFront::baseGameHandle<game::ticTacToeLogic>;
template class gameFront::baseGameHandle<game::checkersLogic>;
//template class gameFront::baseGameHandle<game::baseGameLogic>;

using namespace dpp;

namespace gameFront {

template <typename T>
baseGameHandle<T>::baseGameHandle(std::function<inType*(gameInt::baseGameInt<T>*)> inHandles[2],std::vector<gameTime> inTimes) {
	//std::cout << colorForm("Started",GREEN_TERM) << std::endl;
	gameTime * tCon[3] = {new gameTime(0),new gameTime(0),new gameTime(0)};
	
	//std::cout << "At line: " << colorForm(std::to_string(__LINE__),RED_TERM) << std::endl;
	
	

	const std::function<void(bool,int)> iEnd = ([this](bool winner, int winCase) {
		//std::cout << "game has ended :((\n";
		//std::cout << "At line: " << colorForm(std::to_string(__LINE__),RED_TERM) << std::endl;
		setRates(this->gameHandles[0]->handler->testCon,this->gameHandles[0]->userIdOne,this->gameHandles[0]->userIdTwo,0,winner); // weeeeeeeeeee very lazy
		//std::cout << "At line: " << colorForm(std::to_string(__LINE__),RED_TERM) << std::endl;
		setRates(this->gameHandles[1]->handler->testCon,this->gameHandles[1]->userIdOne,this->gameHandles[1]->userIdTwo,0,!winner); // weeeeeeeeeee very lazy
		//std::cout << "At line: " << colorForm(std::to_string(__LINE__),RED_TERM) << std::endl;
		this->gameHandles[0]->endCall(winner,winCase);
		this->gameHandles[1]->endCall(!winner,winCase);
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



}


}

