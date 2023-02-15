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
        dataHandle(std::string tConOption);

    private: 
        MYSQL * dataCon;
};
}