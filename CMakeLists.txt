cmake_minimum_required(VERSION 3.15)
project(duelBot VERSION 0.1 DESCRIPTION "A discord bot")

OPTION(TOK_BUILD "Token type" ON)
IF (${TOK_BUILD})
    ADD_DEFINITIONS(-DTOKEN_TYPE="BUILD")
ELSE()
    ADD_DEFINITIONS(-DTOKEN_TYPE="TEST")
ENDIF()
if (NOT DEFINED MYSQL_CONN_DIR)
    set(MYSQL_CONN_DIR /usr/local/mysql)
endif()
set(DUEL_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)
find_package( OpenCV REQUIRED )
#find_package(OpenMP)

include_directories(${MYSQL_CONN_DIR}/include)
include_directories(${DUEL_ROOT}/include;${OpenCV_INCLUDE_DIRS})
#file(GLOB_RECURSE SECRETS "secrets/*.hpp")
file(GLOB_RECURSE INC_SOURCES "include/*.cpp") 
add_subdirectory(${DUEL_ROOT}/libs/DPP)

if (EXISTS ${DUEL_ROOT}/secrets/config.json)
    message(STATUS "config.json found")
else()
    message(FATAL_ERROR "config.json not found")
endif()


add_link_options("-lmysqlclient")


add_executable(${PROJECT_NAME}
    ${DUEL_ROOT}/src/main.cpp
    ${INC_SOURCES}
)


target_link_libraries(${PROJECT_NAME}
    dpp
    ${OpenCV_LIBS}
    nlohmann_json::nlohmann_json
    ${MYSQL_CONN_DIR}/lib/libmysqlclient.a
    #OpenMP::OpenMP_CXX

)
 

target_include_directories(${PROJECT_NAME} PUBLIC
    libs/DPP/include
    include/database
    
    include/utils
    include/games
    include/front
    include/draw
    include/interactions
    include/eventhandle
    include/ratetools
    src/commands
    

)


 
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
