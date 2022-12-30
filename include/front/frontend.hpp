#pragma once

#include <dpp/dpp.h>
#include "baseGameInt.hpp"
#include "gameLogic.hpp"
#include <typeindex>
#include <typeinfo>
#include <functional>

using namespace dpp;






namespace gameFront {


	
template <typename T>
class baseThread {
public:
    static_assert(std::is_base_of<game::baseGameLogic,T>::value, "Base game interactions may only have templates of game types");
    baseThread(cluster& botPar, snowflake userIdA, snowflake userIdB, snowflake threadId);
    baseThread(cluster& botPar, snowflake userIdA, snowflake userIdB, snowflake threadId, std::string gameName);
    std::string drawBoard(bool userMove, std::vector<std::vector<int>> boardState);
    std::function<void(message msg)> moveCall;
    void endCall(bool userWin, int winType);

private:
    cluster& bot;
    snowflake userIdOne;
    snowflake userIdTwo;
    snowflake gameThread;
    std::string emojiCode;
    gameInt::baseGameInt<T> gameInteraction;


};



}
