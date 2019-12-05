#!/bin/bash

g++ server.cpp game.cpp clientManager.cpp -lpthread -Wall && gdb a.out