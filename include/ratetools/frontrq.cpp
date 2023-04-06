#include "ratesys.hpp"
#include <thread>
#include <chrono>
#include <dpp/dpp.h>
#include <iostream>
//std::this_thread::sleep_for(*curTime + (*this->timeControl[1]) + (*this->timeControl[2]));
//this->endCase(this->userMove,1);

const std::chrono::duration<double,std::milli> tDelay(5000);

using namespace dpp;
namespace rQ {


frontRQ::frontRQ(std::function<void(snowPair,snowPair)> inFunc,mData::dataHandle * inCon) {
	this->sTempFunc = inFunc;
	this->logicQ = new rateQ(inCon);
	this->gThread = new std::thread([&] {
		while (true) {
			std::cout << "Running!\n";
			std::vector<gBias> curGames = this->logicQ->getGames();
			std::vector<snowPair> curGamesP;
			for (auto a : curGames) {
				curGamesP.push_back(a.players);
			}
			for (auto cG : curGamesP) {
				snowPair curThreads(this->playThreads[cG.first],this->playThreads[cG.second]);
				this->sTempFunc(cG,curThreads);
				this->playThreads.erase(cG.first); this->playThreads.erase(cG.second);
			}
			std::this_thread::sleep_for(tDelay);
		}
	});
			

}


/***
 * @brief adds player to queue from interaction
 * @param inInter the interaction to get thread and player info from
 */
bool frontRQ::addPlayerInt(interaction inInter) {
	snowflake playerAdd = inInter.usr.id;
	snowflake threadAdd = inInter.get_channel().id;
	if (this->playThreads.contains(playerAdd)) { return false; }
	try {
		this->playThreads.emplace(playerAdd,threadAdd);
		this->logicQ->addPlayer(playerAdd);
		return true;
	} catch (...) {
		return false;
	}
	return false;
}




}
