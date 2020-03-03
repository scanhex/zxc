cmake_minimum_required(VERSION 3.15)
project(Client)

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


add_executable(Client main.cpp)

if(Boost_FOUND)
    message(STATUS "Linking boost libraries")
    target_link_libraries(Client ${Boost_LIBRARIES})

endif()