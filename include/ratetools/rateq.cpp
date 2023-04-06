#include <dpp/dpp.h>
#include <vector>
#include "ratesys.hpp"
#include "databaselogic.hpp"
#include <cmath>

#ifndef TW
#define TW 2
#endif
#ifndef MINFIT
#define MINFIT 10
#endif

using namespace dpp;
using snowPair = std::pair<snowflake,snowflake>;

int noZe(int inNum) {
	if (inNum < 1) { return 1; } else { return inNum; }
}

namespace rQ {

rateQ::rateQ(mData::dataHandle * dataCon,int inGameId) { 
	this->rateCon = dataCon; 
	this->gameId = inGameId;
}

void rateQ::addPlayer(snowflake inPlay) {
	rPlay addPlay;
	addPlay.player = inPlay;
	addPlay.rate = getRate(this->rateCon,inPlay,this->gameId);
	addPlay.curW = std::sqrt(std::abs(addPlay.rate-this->meanPlay)/noZe(this->numPlay));
	//root((|r-m|)/n)
	this->joinQ.push_back(addPlay);
	this->curSum += addPlay.rate;
}

std::vector<gBias> rateQ::getGames() {
	std::vector<gBias> retQ = {};
	if ((this->numPlay+this->joinQ.size()) > 0) {
		this->meanPlay = (this->meanPlay * (this->numPlay/(this->numPlay+this->joinQ.size())));
		this->meanPlay += (this->curSum/(this->numPlay+this->joinQ.size()));
	} else {
		this->meanPlay = 10000;
	}
	this->curSum = 0;
	this->numPlay += this->joinQ.size();
	this->curPlay.push_back(this->joinQ);
	this->joinQ = {};
	int curSize = this->curPlay.size();
	for (int i = 0; i < this->curPlay.size(); i++) {
		for (auto &a: this->curPlay[i]) { a.curW = std::sqrt(std::abs(a.rate-this->meanPlay)/noZe(this->numPlay)); }
	}

	//This is the outer loop :(
	for (int i = 0; i < this->curPlay.size(); i++) {
	for (int ii = 0; ii < this->curPlay[i].size(); ii++) {

	for (int j = i; j < this->curPlay.size(); j++) {
		int start = 0;
		if ((j - i) == 0) start = ii + 1;
		

		for (int jj = start; jj < this->curPlay[j].size(); jj++) {
			double curBias = getBiasRaw(this->curPlay[i][ii].rate,this->curPlay[j][jj].rate);
			if ((((std::pow((TW*(curSize-i)),2)/curBias))*this->curPlay[i][ii].curW >= MINFIT)
			and (((std::pow(TW*(curSize-j),2)/curBias))*this->curPlay[j][jj].curW >= MINFIT)) {
				gBias nVal;
				nVal.players = {this->curPlay[i][ii].player,this->curPlay[j][jj].player};
				nVal.bias = (int)(curBias);
				retQ.push_back(nVal);
				this->curPlay[i].erase(this->curPlay[i].begin()+ii);
				this->curPlay[j].erase(this->curPlay[jj].begin()+ii);
			}
		}
	}}}
	return retQ;

			

	

}



}

