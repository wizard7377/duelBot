#pragma once

#include "databaselogic.hpp"
#include <dpp/dpp.h>

int getBias(int valOne, int valTwo, int dScore = 10);

int getRate(mData::dataHandle * dataCon,dpp::snowflake userVal,int gameVal,dpp::snowflake guildVal = 0);