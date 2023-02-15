#pragma once

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
/* uncomment for applications that use vectors */
/*#include <vector>*/

#include <mysql.h>



namespace mData {
class dataHandle {
    public:
        dataHandle();
        uint64_t getUser(uint64_t userId,uint64_t guildId = 0);

    private: 
        MYSQL * dataCon;
};
}