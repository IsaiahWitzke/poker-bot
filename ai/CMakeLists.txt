cmake_minimum_required(VERSION 3.19.4)

project(poker-bot 1.0)

configure_file(PokerBot.h.in PokerBot.h)

add_executable(PokerBot pokerbot.cxx)