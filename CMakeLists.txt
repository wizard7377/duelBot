cmake_minimum_required(VERSION 3.15)
project(duelBot VERSION 0.1 DESCRIPTION "A discord bot")

#Add directories 

include_directories(secrets;include)
file(GLOB_RECURSE GAME_HEADERS "include/games/*.hpp")
file(GLOB_RECURSE GAME_SOURCES "include/games/*.cpp")
file(GLOB_RECURSE SECRETS "secrets/*.hpp")
# Add DPP as dependency
add_subdirectory(libs/DPP)
# You can also add any other libs you want to use


# Create an executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    ${SECRETS}
    ${GAME_SOURCES}
    # your other files...
)
 
# Linking libraries
target_link_libraries(${PROJECT_NAME}
    dpp

 
    # Add any other libs you want to use here
)
 
# Specify includes
target_include_directories(${PROJECT_NAME} PRIVATE
    libs/DPP/include
    include/games
    # Remember to add the include directories of any other libraries too
)
 
# Set C++ version
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)