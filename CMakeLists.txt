cmake_minimum_required(VERSION 3.15)
project(duelBot VERSION 0.1 DESCRIPTION "A discord bot")

set(FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR /usr/include/mysql)


include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)
find_package( OpenCV REQUIRED )
#find_package(OpenMP)

include_directories(${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR})
include_directories(include;${OpenCV_INCLUDE_DIRS})
#file(GLOB_RECURSE SECRETS "secrets/*.hpp")
file(GLOB_RECURSE INC_SOURCES "include/*.cpp") 
add_subdirectory(libs/DPP)


add_link_options("-lmysqlclient")


add_executable(${PROJECT_NAME}
    src/main.cpp
    ${INC_SOURCES}
)


target_link_libraries(${PROJECT_NAME}
    dpp
    ${OpenCV_LIBS}
    nlohmann_json::nlohmann_json
    /usr/lib/x86_64-linux-gnu/libmysqlclient.a
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
