cmake_minimum_required(VERSION 3.15)
project(duelBot VERSION 0.1 DESCRIPTION "A discord bot")

#Add directories 


include_directories(secrets;include)
file(GLOB_RECURSE FRONT_SOURCES "include/front/*.cpp")
file(GLOB_RECURSE GAME_HEADERS "include/games/*.hpp")
file(GLOB_RECURSE GAME_SOURCES "include/games/*.cpp")
file(GLOB_RECURSE INTER_SOURCES "include/interactions/*.cpp")
file(GLOB_RECURSE COMMAND_SOURCES "include/src/commands/*.cpp")
file(GLOB_RECURSE DATA_SOURCES "include/database/*.cpp")
file(GLOB_RECURSE SECRETS "secrets/*.hpp")
# Add DPP as dependency
add_subdirectory(libs/DPP)
# You can also add any other libs you want to use



# Create an executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    ${SECRETS}
    ${DATA_SOURCES}
    ${GAME_SOURCES}
    ${COMMAND_SOURCES}
    ${INTER_SOURCES}
    ${FRONT_SOURCES}
    
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
    include/database
    include/front
    include/interactions
    src/commands

    # Remember to add the include directories of any other libraries too
)
 
# Set C++ version
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
