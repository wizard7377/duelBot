#pragma once

#include "databaselogic.hpp"
#include <dpp/dpp.h>

using namespace dpp;

int getBias(int valOne, int valTwo, int dScore = 10);

int getRate(mData::dataHandle * dataCon,dpp::snowflake userVal,int gameVal,dpp::snowflake guildVal = 0);

int setRates(mData::dataHandle * dataCon,snowflake setPlayer, snowflake otherPlayer, int gameVal, bool isWinner = true, snowflake guildVal = 0,snowflake otherGuild = 0);