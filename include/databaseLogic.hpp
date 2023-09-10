#pragma once

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "databaseTypes.hpp"

#include <mysql.h>



namespace mData {

/*!
 * Wrapper around C MySQL API
 * @class dataHandle
 * @brief MySQL wrapper
*/
class dataHandle {
    public:
        dataHandle();
        //tbh i really need to update this
        /*! Gets rating of player in given guild
         * @brief Gets rating
         * @return Size one vector of int variant
         * @param gameId Game being checked
         * @param userId User being checked (snowflake)
         * @param guildId Guild being checked, blank for outside guild (snowflake)
         */
        reSet getRate(int gameId,uint64_t userId,uint64_t guildId = 0);
        /*! Sets rating of player in given guild
         * @brief Sets rating
         * @param reqRate New rating being requested
         * @param gameId Game being set
         * @param userId User being set (snowflake)
         * @param guildId Guild being set, blank for outside guild (snowflake)
         */
        void setRate(int reqRate,int gameId,uint64_t userId,uint64_t guildId = 0);
        /*! Sets given setting in given guild in given scopes
         * @brief Sets setting
         * @param setName Setting being requested
         * @param val Value being set to
         * @param gameId Game being set
         * @param userId User being set (snowflake)
         * @param guildId Guild being set, blank for outside guild (snowflake)
         * @param scopeSet Scope of setting change 
         * | Value | Scope | 
         * |-------|-------| 
         * |0|Only said item|
         * |1|Only outside guilds|
         * |2|In or outside specified guild|
         * |3|All guilds|
         * |4|Only inside guilds|
         */
        bool editSetting(string setName, string val, int gameId, uint64_t userId, uint64_t guildId = 0, int scopeSet = 0);
        /*! Execute a given MySQL statement
         * @brief Execute statement
         * @return Rows and columns
         * @param query Statement
         * @param retType Whether to return values
         */
        std::vector<std::vector<std::string>> execQ(std::string query, bool retType = false);
    private: 
        MYSQL * dataCon; //!< C API connection
        /*!
         * @brief Get the user database id
         * 
         * @param userId Given user snowflake
         * @param guildId Given guild snowflake (default global)
         * @return reSet Database id 
         */
        reSet getUser(uint64_t userId,uint64_t guildId = 0);
        reSet makeGUserRaw(int gameId,uint64_t totalId);
        
};
}