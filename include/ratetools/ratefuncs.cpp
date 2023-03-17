#include "ratesys.hpp"
#include "databaselogic.hpp"
#include <dpp/dpp.h>
#include <cmath>
#include <variant>

#define DCOF 2

int getBias(int valOne, int valTwo, int dScore) {
	if (valOne < valTwo) { int tVal = valTwo; valTwo = valOne; valOne = tVal; }
	double fVal = ((valOne - std::sqrt(valOne * valTwo))/(DCOF*(valOne+valTwo)));
	return ((int)(dScore*fVal));
}


/*

int getRate(mData::dataHandle * dataCon, dpp::snowflake userVal, int gameVal, dpp::snowflake guildVal) {
	uint64_t retVal = 0;
	if ((uint64_t)(guildVal) == 0) { retVal = std::get<uint64_t>(dataCon->getUser(userVal)[0]); }
	else { retVal = std::get<uint64_t>(dataCon->getUser(userVal,guildVal)[0]); }
	
	std::string curQ = ("SELECT gameRate FROM userGameInfo WHERE gameUserId = " + std::to_string(retVal) + " AND gameId = " + std::to_string(gameVal) + ";");
	//std::string res = dataCon->execQ(curQ,0)[0][0];
	auto resBig = dataCon->execQ(curQ,0);
	std::string res = resBig[0][0];	
	return (std::stoi(res));
}

*/



int getRate(mData::dataHandle * dataCon, dpp::snowflake userVal, int gameVal, dpp::snowflake guildVal) {
	uint64_t retVal = std::get<int>(dataCon->getRate(gameVal,userVal,guildVal)[0]);
	return retVal;
}
