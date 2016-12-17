#ifndef ChessBoard_hpp
#define ChessBoard_hpp 1

#pragma once

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>

#include <vector>
#include <string>
#include <iostream>
#include <forward_list>
#include <stdint.h>
#include <vector>
#include <array>
#include <iterator>

struct MoveStr {
	uint8_t iStart;
	uint8_t jStart;
	uint8_t iEnd;
	uint8_t jEnd;
	uint8_t movingPiece;
	uint8_t capturedPiece;
};
struct pieceCoords {
	uint8_t i;
	uint8_t j;
};
//struct listOfPieces {
//	
//	std::array<vector<pieceCoords>, 13> pieces;
//	//std::vector<pieceCoords>& wPawns, wRooks, wBishops, wKnights, wQueens, wKings;
//	//std::vector<pieceCoords>& bPawns, bRooks, bBishops, bKnights, bQueens, bKings;
//
//};

class AbstractBoard
{
public:
	// virtual vector<AbstractBoard>& generatePossibleMoves() = 0;
	// virtual int32_t evaluateBoard() = 0;

};


class HashedBoard
{
public:
	// bestMoveFlagsDepth is a 32 bit int ordered with
	// [32:24 is 0] [24:12 is move encoding] [12:4 is depth] [4:0 are the flags]
	const int EXACTTYPE = 2;
	const int ALPHATYPE = 4;
	const int BETATYPE = 8;

	int64_t getKey() { return key; }
	int32_t getEval() { return eval; }
	int32_t getDepth() { return (bestMoveFlagsDepth >> 4) & 0xFF; }
	bool ancient() { return bestMoveFlagsDepth & 0x01; }

	int32_t getType() { return bestMoveFlagsDepth & 0x0E; }
	int getBestMove() { return bestMoveFlagsDepth >> 12; }
private:
	int64_t key;
	int32_t eval;
	int32_t bestMoveFlagsDepth;

};

class ChessBoard : public AbstractBoard
{
public:

	ChessBoard();
	ChessBoard(const int32_t squares[8][8]);
	//ChessBoard(const ChessBoard& copy);
	void makeMove(MoveStr move);
	void unMakeMove(MoveStr move);
	std::vector<MoveStr> generatePossibleMoves();// override;
	int32_t evaluateBoard();
	friend std::ostream& operator<<(std::ostream&, const ChessBoard&);
	friend std::istream& operator>>(std::istream& in, ChessBoard&);
	static void initRandomHash();
	static int64_t moveHashes[64][13];


	// static const uint8_t WHITE = 0x80;
	// static const uint8_t BLACK = 0x00;
	// static const uint8_t EMPTY = 0x00;
	// static const uint8_t PAWN =   'p'; 
	// static const uint8_t KNIGHT = 'n';
	// static const uint8_t BISHOP = 'b';
	// static const uint8_t ROOK =   'r';
	// static const uint8_t QUEEN =  'q';
	// static const uint8_t KING =   'k';


	static int32_t defaultStart[8][8];

	static const int32_t pawnMobilityWeight = 2;
	static const int32_t pawnMaterialWeight = 100;
	static const int32_t rookMobilityWeight = 1;
	static const int32_t rookMaterialWeight = 500;
	static const int32_t knightMobilityWeight = 8;
	static const int32_t knightMaterialWeight = 325;
	static const int32_t bishopMobilityWeight = 6;
	static const int32_t bishopMaterialWeight = 330;
	static const int32_t queenMobilityWeight = 5;
	static const int32_t queenMaterialWeight = 1000;
	static const int32_t kingMobilityWeight = 1;
	static const int32_t kingMaterialWeight = 100000;

	//private:
	uint8_t _colorToMove;
	bool _virginWhiteKing;
	bool _virginBlackKing;
	int _moveIsQuiet;
	int64_t _zobristKey;
	uint8_t _i, _j;
	int32_t _squares[8][8];

	std::array<std::vector<pieceCoords>, 13> _pieceList; // 8 for 8 pieces per bin and 13 for hasho 12/13 types
	int32_t _uniqueIndex[8][8];

	std::vector<MoveStr> _moveHistory;
	std::vector<int> _capturedPieces;
	void addMoveToList(uint8_t, uint8_t, int, std::vector<MoveStr>*);
	void addMoveWithPromotionsToList(uint8_t, uint8_t, int, std::vector<MoveStr>*);
	int addPawnMoves(std::vector<MoveStr>*);
	int addRookMoves(std::vector<MoveStr>*);
	int addKnightMoves(std::vector<MoveStr>*);
	int addBishopMoves(std::vector<MoveStr>*);
	int addQueenMoves(std::vector<MoveStr>*);
	int addKingMoves(std::vector<MoveStr>*);
	//uint8_t countKingMoves();
	inline bool inBounds(int i, int j);
	int32_t scoreMovesAndMaterial(uint8_t moveColor);
	int64_t genHashKey();
	void setupPieceLists();

	inline int getIndexFromChar(int chessPiece);


};



#endif



