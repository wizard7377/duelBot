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
baseGameHandle<T>::baseGameHandle(std::function<inType*(gameInt::baseGameInt<T>*)> inHandles[2]) {
	gameInt::gameTimeType * tCon[3] = {new gameInt::gameTimeType(0),new gameInt::gameTimeType(0),new gameInt::gameTimeType(0)};
	const std::function<void(bool,int)> iEnd = ([](bool idkOne, int idkTwo) {
		std::cout << "game has ended :((\n";
	}); 
	this->shState = new gameInt::baseGameInt<T>(tCon,iEnd);
	for (int i = 0; i <= 1; i++) {
		this->gameHandles[i] = inHandles[i](shState);
	}

	this->gameHandles[0]->giveMove = ( [this] {this->gameHandles[1]->getMove(); } );
	this->gameHandles[1]->giveMove = ( [this] {this->gameHandles[0]->getMove(); } );



}


}

