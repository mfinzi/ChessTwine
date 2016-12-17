#pragma once

#ifndef Engine_h
#define Engine_h 1
#include <vector>
#include <stdlib.h>

template <class gameBoard> class Engine
{
public:
	//private:
	void reOrderMoves(std::vector<MoveStr>& moveList);
	int32_t negamax(gameBoard& thisBoard, int depth, int32_t alpha, int32_t beta);
	//int32_t searchForQuiet(gameBoard& thisBoard, int32_t alpha, int32_t beta);

};
#endif