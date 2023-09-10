cmake_minimum_required(VERSION 3.27)
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
find_package(spdlog REQUIRED)
#find_package(OpenMP)
include(ExternalProject)
FetchContent_Declare(DPP
    PREFIX libs/DPP
    BINARY_DIR build
    URL https://github.com/brainboxdotcc/DPP/archive/refs/tags/v10.0.25.tar.gz
)
FetchContent_MakeAvailable(DPP)
if (EXISTS ${MYSQL_CONN_DIR}/include)
    include_directories(${MYSQL_CONN_DIR}/include)
else()
    include_directories(/usr/include/mysql)
endif()
include_directories(${DUEL_ROOT}/include;${OpenCV_INCLUDE_DIRS})
#file(GLOB_RECURSE SECRETS "secrets/*.hpp")
file(GLOB_RECURSE INC_SOURCES "src/*.cpp") 
#add_subdirectory(${DPP_BINARY_DIR})

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
    spdlog::spdlog
    #OpenMP::OpenMP_CXX
)
if (EXISTS ${MYSQL_CONN_DIR}/lib/libmysqlclient.a) 
	target_link_libraries(${PROJECT_NAME} ${MYSQL_CONN_DIR}/lib/libmysqlclient.a)
elseif (EXISTS ${MYSQL_CONN_DIR}/lib/libmysqlclient.so) 
	target_link_libraries(${PROJECT_NAME} ${MYSQL_CONN_DIR}/lib/libmysqlclient.so)
else() 
	target_link_libraries(${PROJECT_NAME} /usr/lib/libmysqlclient.so)
endif()
#add_dependencies(${PROJECT_NAME} DPP)
 

target_include_directories(${PROJECT_NAME} PUBLIC
    libs/DPP/include
    include
    

)


 
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
