SRC_PROJECT = main.cpp
DEBUG_TARGET = bin/debug/out
RELEASE_TARGET = bin/release/out
INSTALL_PATH = /usr/local/bin
default:
debug:
	mkdir -p bin/debug
	g++ -fdiagnostics-color=always -std=c++20 -g $(SRC_PROJECT) -o $(DEBUG_TARGET)

release:
	mkdir -p bin/release
	g++ -std=c++20 $(SRC_PROJECT) -o $(RELEASE_TARGET)