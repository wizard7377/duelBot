#pragma once

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>
/* uncomment for applications that use vectors */
#include <vector>

#include "dbtypes.hpp"

#include <mysql.h>



namespace mData {
class dataHandle {
    public:
        dataHandle();
        //tbh i really need to update this
        reSet getUser(uint64_t userId,uint64_t guildId = 0);
        reSet getRate(int gameId,uint64_t userId,uint64_t guildId = 0);
        void setRate(int reqRate,int gameId,uint64_t userId,uint64_t guildId = 0);
        bool editSetting(string setName, string val, int gameId, uint64_t userId, uint64_t guildId = 0, int scopeSet = 0);
        std::vector<std::vector<std::string>> execQ(std::string query, bool retType = false);
    private: 
        MYSQL * dataCon;
        reSet makeGUserRaw(int gameId,uint64_t totalId);
        
};
}