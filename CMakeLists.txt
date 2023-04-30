cmake_minimum_required(VERSION 3.15)
project(duelBot VERSION 0.1 DESCRIPTION "A discord bot")

set(FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR /usr/local/mysql)


include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)
find_package( OpenCV REQUIRED )
#find_package(OpenMP)

include_directories(${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR}/include)
include_directories(include;${OpenCV_INCLUDE_DIRS})
#file(GLOB_RECURSE SECRETS "secrets/*.hpp")
file(GLOB_RECURSE INC_SOURCES "include/*.cpp") 
add_subdirectory(libs/DPP)

if (EXISTS secrets/config.json)
    message(STATUS "config.json found")
else()
    message(FATAL_ERROR "config.json not found")
endif()


#add_link_options("-lmysqlclient")


add_executable(${PROJECT_NAME}
    src/main.cpp
    ${INC_SOURCES}
)


target_link_libraries(${PROJECT_NAME}
    dpp
    ${OpenCV_LIBS}
    nlohmann_json::nlohmann_json
    ${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR}/lib/libmysqlclient.a
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
