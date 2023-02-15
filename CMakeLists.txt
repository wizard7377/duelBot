cmake_minimum_required(VERSION 3.15)
project(duelBot VERSION 0.1 DESCRIPTION "A discord bot")

set(FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR /usr/include/mysql-cppconn-8)

#Add directories 
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)
find_package( OpenCV REQUIRED )
#find_package(MySQL REQUIRED)
include_directories(${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR})
include_directories(secrets;include;${OpenCV_INCLUDE_DIRS})
file(GLOB_RECURSE SECRETS "secrets/*.hpp")
file(GLOB_RECURSE INC_SOURCES "include/*.cpp") 
# Add DPP as dependency
add_subdirectory(libs/DPP)
# You can also add any other libs you want to use
#set(CMAKE_CXX_FLAGS "-O0")


add_link_options("-lmysqlcppconn8")

# Create an executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    ${SECRETS}
    ${INC_SOURCES}
)








 
# Linking libraries
target_link_libraries(${PROJECT_NAME}
    dpp
    ${OpenCV_LIBS}
    nlohmann_json::nlohmann_json
    /usr/lib/x86_64-linux-gnu/libmysqlcppconn8.so
    #${MYSQL_LIBRARY}



 
    # Add any other libs you want to use here
)
 
# Specify includes

target_include_directories(${PROJECT_NAME} PUBLIC
    libs/DPP/include
    include/utils
    include/games
    include/database
    include/front
    include/draw
    include/interactions
    include/eventhandle
    src/commands
    

    # Remember to add the include directories of any other libraries too
)


 
# Set C++ version
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
