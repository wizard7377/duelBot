#pragma once

#include "databaselogic.hpp"
#include <dpp/dpp.h>
#include <unordered_map>
#include <map>
#include <thread>
#include <functional>


using namespace dpp;
using snowPair = std::pair<snowflake,snowflake>;

struct gBias {
	snowPair players;
	int bias;
};
struct rPlay {
	snowflake player;
	int rate;
    double curW;
};

int getBias(int valOne, int valTwo, int dScore = 10);
double getBiasRaw(int valOne, int valTwo, int dScore = 10);
int getRate(mData::dataHandle * dataCon,dpp::snowflake userVal,int gameVal,dpp::snowflake guildVal = 0);

int setRates(mData::dataHandle * dataCon,snowflake setPlayer, snowflake otherPlayer, int gameVal, bool isWinner = true, snowflake guildVal = 0,snowflake otherGuild = 0);

namespace rQ {

class rateQ {
	public:
		rateQ(mData::dataHandle * dataCon,int inGameId = 0);
		std::vector<gBias> getGames();
		void addPlayer(snowflake inPlay);
	private:
		std::vector<std::vector<rPlay>> curPlay = {};
        std::vector<rPlay> joinQ = {};
        mData::dataHandle * rateCon;
        int numPlay = 0;
        double meanPlay = 0;
        uint64_t curSum = 0;
        int gameId;


};


class frontRQ {
	public:
		frontRQ(std::function<void(snowPair,snowPair)> inFunc,mData::dataHandle * inCon);
		bool addPlayerInt(dpp::interaction inInter);
	private:
		std::function<void(snowPair,snowPair)> sTempFunc;
		std::thread * gThread;
		rateQ * logicQ;
		std::unordered_map<snowflake,snowflake> playThreads = {};
};

}
