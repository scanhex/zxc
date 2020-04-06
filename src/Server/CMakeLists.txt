cmake_minimum_required(VERSION 3.15)
project(Server)

set(CMAKE_CXX_STANDARD 17)

message(STATUS "start running cmake...")
set(CMAKE_CXX_FLAGS "-lpthread")

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)


find_package(Boost 1.65.0 COMPONENTS system thread REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost found")
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(Server main.cpp Server.cpp Server.h
        ../Utils/BufferIO.cpp ../Utils/BufferIO.h
        ../Game/Hero.cpp ../Game/Hero.h
        ../Game/Skill.cpp ../Game/Skill.h
        ../Game/Item.cpp ../Game/Item.h
        ../Game/Buff.cpp ../Game/Buff.h
        ../Game/Unit.cpp ../Game/Unit.h
        ../Game/Point.cpp ../Game/Point.h
        ../Game/Stats.cpp ../Game/Stats.h
        ../Game/StatsBuilder.cpp ../Game/StatsBuilder.h
        ../Game/GameState.cpp ../Game/GameState.h
        ../Game/GameStateServer.h
        ../Game/Event.h)

if(Boost_FOUND)
    message(STATUS "Linking boost libraries")
    target_link_libraries(Server ${Boost_LIBRARIES})

endif()
