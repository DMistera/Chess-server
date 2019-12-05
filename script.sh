#!/bin/bash

g++ server.cpp game.cpp clientManager.cpp gamestate.cpp piece.cpp pawn.cpp rook.cpp queen.cpp king.cpp knight.cpp bishop.cpp field.cpp move.cpp -lpthread -Wall && ./a.out