cmake_minimum_required(VERSION 3.12)
project(CS205Project_GoFish_Game)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(CS205Project_GoFish_Game
        Card.cpp
        Card.h
        Deck.cpp
        Deck.h
        Game.cpp
        Game.h
        main.cpp
        testing.cpp)
