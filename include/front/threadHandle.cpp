#include "frontend.hpp"
#include <dpp/dpp.h>
#include <concepts>
#include <functional>
#include "baseGameInt.hpp"

template class gameFront::baseGameHandle<game::ticTacToeLogic>;
template class gameFront::baseGameHandle<game::checkersLogic>;
//template class gameFront::baseGameHandle<game::baseGameLogic>;

using namespace dpp;

namespace gameFront {

template <typename T>
baseGameHandle<T>::baseGameHandle(std::function<inType*(gameInt::baseGameInt<T>*)> inHandles[2],std::vector<gameTime> inTimes) {
	gameTime * tCon[3] = {new gameTime(0),new gameTime(0),new gameTime(0)};
	const std::function<void(bool,int)> iEnd = ([this](bool winner, int winCase) {
		std::cout << "game has ended :((\n";
		this->gameHandles[0]->endCall(winner,winCase);
		this->gameHandles[1]->endCall(!winner,winCase);
	}); 
	this->shState = new gameInt::baseGameInt<T>(inTimes.data(),iEnd);
	for (int i = 0; i <= 1; i++) {
		this->gameHandles[i] = inHandles[i](shState);
	}

	this->gameHandles[0]->giveMove = ( [this] {this->gameHandles[1]->getMove(); } );
	this->gameHandles[1]->giveMove = ( [this] {this->gameHandles[0]->getMove(); } );



}


}

