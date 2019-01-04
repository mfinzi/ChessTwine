

#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <forward_list>
#include <stdint.h>
#include <vector>
#include <array>


#include "ChessBoard.h"
#include "Engine.h"

template <class gameBoard>
void Engine<gameBoard>::reOrderMoves(std::vector<MoveStr>& moveList) {
	std::random_shuffle(moveList.begin(), moveList.end());
}

// template <class gameBoard>
// int32_t Engine<gameBoard>::searchForQuiet(gameBoard& thisBoard, int32_t alpha, int32_t beta){
// 	int32_t nodeScore = thisBoard.evaluateBoard();
// 	if (nodeScore >= beta) return beta;
// 	if (nodeScore > alpha) alpha = nodeScore;
// 	vector<gameBoard> movesWithCapture = generateCaptures();
// 	for(auto it = movesWithCapture.begin(); it!= movesWithCapture.end(); ++it){
// 	}
// }

template <class gameBoard>
int32_t Engine<gameBoard>::negamax(gameBoard& thisBoard, int depth, int32_t alpha, int32_t beta) {
	int32_t wSgn = thisBoard._colorToMove * 2 - 1;
	if (depth == 0) { // Or king has been taken
		return thisBoard.evaluateBoard();
	}
	int32_t nodeScoreLowerBound = -10000000;

	std::vector<MoveStr> moveList = thisBoard.generatePossibleMoves();
	reOrderMoves(moveList);
	for (auto it = moveList.begin(); it != moveList.end(); ++it) {
		thisBoard.makeMove(*it);
		int32_t childNodeScore = -1 * negamax(thisBoard, depth - 1, -1 * beta, -1 * alpha);
		nodeScoreLowerBound = std::max(nodeScoreLowerBound, childNodeScore);
		alpha = std::max(alpha, childNodeScore);
		thisBoard.unMakeMove(*it);
		if (alpha>beta) break; // Alpha beta cutoff, no need to further examine childs of this node
	}
	return nodeScoreLowerBound;
}

