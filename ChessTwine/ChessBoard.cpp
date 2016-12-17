#include "stdafx.h"
#include "ChessBoard.h"
#include "Engine.h"
#include <algorithm>
#include <ctime>
#define PIECEMASK 0x7F
#define COLORMASK 0x80
#define WHITE 0x80
#define BLACK 0x00
#define EMPTY 0x00
#define PAWN 'P'
#define KNIGHT 'N'
#define BISHOP 'B'
#define ROOK 'R'
#define QUEEN 'Q'
#define KING 'K'
using namespace std;

int64_t ChessBoard::moveHashes[64][13];

int32_t ChessBoard::defaultStart[8][8] = {
	{ WHITE | ROOK,WHITE | KNIGHT,WHITE | BISHOP,WHITE | QUEEN,WHITE | KING,WHITE | BISHOP,WHITE | KNIGHT,WHITE | ROOK },
	{ WHITE | PAWN,WHITE | PAWN  ,WHITE | PAWN  ,WHITE | PAWN ,WHITE | PAWN,WHITE | PAWN  ,WHITE | PAWN  ,WHITE | PAWN },
	{ EMPTY		,EMPTY       ,EMPTY       ,EMPTY      ,EMPTY     ,EMPTY       ,EMPTY       ,EMPTY },
	{ EMPTY     ,EMPTY       ,EMPTY       ,EMPTY      ,EMPTY     ,EMPTY       ,EMPTY       ,EMPTY },
	{ EMPTY     ,EMPTY       ,EMPTY       ,EMPTY      ,EMPTY     ,EMPTY       ,EMPTY       ,EMPTY },
	{ EMPTY     ,EMPTY       ,EMPTY       ,EMPTY      ,EMPTY     ,EMPTY       ,EMPTY       ,EMPTY },
	{ BLACK | PAWN,BLACK | PAWN  ,BLACK | PAWN  ,BLACK | PAWN ,BLACK | PAWN,BLACK | PAWN  ,BLACK | PAWN  ,BLACK | PAWN },
	{ BLACK | ROOK,BLACK | KNIGHT,BLACK | BISHOP,BLACK | QUEEN,BLACK | KING,BLACK | BISHOP,BLACK | KNIGHT,BLACK | ROOK }
};

int32_t testBoard[8][8] = {
	{ 0,0,0,0,0,0,0,BLACK | QUEEN },
	{ 0,0,WHITE | PAWN,0,0,WHITE | BISHOP,WHITE | QUEEN,0 },
	{ 0,0,BLACK | PAWN,0,0,0,0,0 },
	{ 0,BLACK | PAWN,BLACK | KING,0,BLACK | PAWN,0,0,0 },
	{ 0,WHITE | KNIGHT,BLACK | PAWN,WHITE | PAWN,0,0,BLACK | PAWN,0 },
	{ WHITE | BISHOP,0,0,0,0,WHITE | KING,0,BLACK | PAWN },
	{ BLACK | BISHOP,WHITE | ROOK,WHITE | PAWN,0,0,0,0,0 },
	{ BLACK | ROOK,WHITE | KNIGHT,0,BLACK | ROOK,0,0,BLACK | BISHOP,0 }
};


int main() {

	ChessBoard::initRandomHash();
	srand(unsigned(time(0)));
	ChessBoard startB;
	ChessBoard board; board._colorToMove = BLACK;
	//Engine<ChessBoard> myEngine;
	// for(int y=0; y<4; ++y){
	// 	vector<MoveStr> firstMoveList = board.generatePossibleMoves();
	// 	vector<int> moveScores;
	// 	for (auto it = firstMoveList.begin(); it!=firstMoveList.end(); ++it){
	// 		board.makeMove(*it);
	// 		int32_t thisScore = myEngine.negamax(board,5,-10000000,10000000);
	// 		moveScores.push_back(thisScore);
	// 		cout<<"scored as "<<thisScore<<endl;
	// 		cout<<board<<endl<<endl;
	//		board.unMakeMove(*it);
	// 	}
	// 	int bestI = distance(moveScores.begin(),max_element(moveScores.begin(),moveScores.end()));
	// 	board.makeMove(firstMoveList[bestI]);
	// 	cout<<"We Chose"<<board<<"Scored as "<<moveScores[bestI]<<endl<<endl;
	// }


	// Move generation benchmarks
	cout << board << endl << endl;
	printf("_________________\n\n");
	int64_t numMoves = 0;

	vector<MoveStr> moveList = board.generatePossibleMoves();
	clock_t start = clock(), diff;
	double dur = 0;

	for (; dur<1;) {
		board = startB;
		for (int i = 0; i<30; ++i) {
			numMoves += distance(moveList.begin(), moveList.end());
			random_shuffle(moveList.begin(), moveList.end());
			board.makeMove(moveList.front());
			moveList = board.generatePossibleMoves();
		}
		diff = clock() - start;
		dur = diff / (double)CLOCKS_PER_SEC;
	}


	cout << "duration is " << dur << " for " << numMoves;


	//cout<<moveList.front();
	// //cout<<start.scoreMovesAndMaterial(BLACK)<<endl;
	// // array<char,4> bob = start._moveHistory.front();
	// // cout<<bob[0]<<bob[1]<<bob[2]<<bob[3]<<endl;
	return 0;
}
ChessBoard::ChessBoard()
	:_colorToMove{ WHITE }, _virginWhiteKing{ true }, _virginBlackKing{ true }, _moveIsQuiet{ 0 }
{
	copy(&defaultStart[0][0], &defaultStart[0][0] + 64, &_squares[0][0]);

	_zobristKey = genHashKey();
	setupPieceLists();

}

ChessBoard::ChessBoard(const int32_t squares[8][8])
	:_colorToMove{ WHITE }, _virginWhiteKing{ true }, _virginBlackKing{ true }, _moveIsQuiet{ 0 }
{
	copy(&squares[0][0], &squares[0][0] + 64, &_squares[0][0]);
	_zobristKey = genHashKey();
	setupPieceLists();
}
//Not needed
//ChessBoard::ChessBoard(const ChessBoard& toCopy)
//	:_colorToMove{ toCopy._colorToMove }, _virginWhiteKing{ toCopy._virginWhiteKing },
//	_virginBlackKing{ toCopy._virginBlackKing }, _moveIsQuiet{ toCopy._moveIsQuiet },
//	_zobristKey{ toCopy._zobristKey }
//{
//	copy(&toCopy._squares[0][0], &toCopy._squares[0][0] + 64, &_squares[0][0]);
//}

ostream& operator<<(ostream& ost, const ChessBoard& cb) {
	ost << "   A  B  C  D  E  F  G  H  \n";
	ost << " --------------------------\n";
	for (uint8_t i = 0; i<8; ++i) {
		ost << (i + 1) << '|';
		for (uint8_t j = 0; j<8; ++j) {
			if ((cb._squares[i][j] & PIECEMASK) == EMPTY) ost << "   ";
			else {
				ost << " " << (cb._squares[i][j] & COLORMASK ? 'w' : 'b');
				ost << (char(cb._squares[i][j] & PIECEMASK));
			}
		}
		ost << '\n';
	}
	return ost;
}

istream& operator>>(istream& in, ChessBoard& cb) {
	uint8_t startColumn, startRow, endColumn, endRow;
	in >> startColumn >> startRow >> endColumn >> endRow;
	//cb.move(startRow-'1',startColumn-'a',endRow-'1',endColumn-'a');
	return in;
}


int32_t ChessBoard::evaluateBoard() {
	int32_t boardScore = 0;
	boardScore += scoreMovesAndMaterial(_colorToMove);
	return boardScore;

}



int32_t ChessBoard::scoreMovesAndMaterial(uint8_t moveColor) {
	int32_t score = 0;
	for (_i = 0; _i<8; ++_i) {
		for (_j = 0; _j<8; ++_j) {
			int32_t wSgn = ((_squares[_i][_j] & COLORMASK) != moveColor) * 2 - 1; // Should be +1 for same, -1 for diff
			switch (_squares[_i][_j] & PIECEMASK) {
			case PAWN: {
				score += wSgn * pawnMobilityWeight * addPawnMoves(NULL);
				score += wSgn * pawnMaterialWeight; break;
			}
			case ROOK: {
				score += wSgn * rookMobilityWeight * addRookMoves(NULL);
				score += wSgn * rookMaterialWeight; break;
			}
			case KNIGHT: {
				score += wSgn * knightMobilityWeight * addKnightMoves(NULL);
				score += wSgn * knightMaterialWeight; break;
			}
			case BISHOP: {
				score += wSgn * bishopMobilityWeight * addBishopMoves(NULL);
				score += wSgn * bishopMaterialWeight; break;
			}
			case QUEEN: {
				score += wSgn * queenMobilityWeight * addQueenMoves(NULL);
				score += wSgn * queenMaterialWeight; break;
			}
			case KING: {
				score += wSgn * kingMobilityWeight * addKingMoves(NULL);
				score += wSgn * kingMaterialWeight; break;
			}
			}
		}
	}
	return score;
}

void ChessBoard::initRandomHash() {
	for (int i = 0; i<64; ++i) {
		for (int j = 0; j<13; ++j) {
			// Ensures 64 bit uniform random numbers
			int64_t rand32 = (rand() << 16) | rand();
			int64_t secondRand32 = (rand() << 16) | rand();
			moveHashes[i][j] = (rand32 << 32) | secondRand32;
		}
	}
}
// Small 13 number hash of the characters, excludes 6
int ChessBoard::getIndexFromChar(int chessPiece) {
	return ((chessPiece ^ 20) & 215) % 15;

}
int64_t ChessBoard::genHashKey() {
	int64_t fullKey = 0;
	for (int i = 0; i<64; ++i) {
		int pieceIndex = getIndexFromChar(_squares[i / 8][i % 8]);
		int64_t pieceHash = moveHashes[i][pieceIndex] ? 0 : moveHashes[i][pieceIndex];
		fullKey ^= pieceHash;

	}
	fullKey ^= _colorToMove ? moveHashes[0][6] : 0;
	fullKey ^= _virginWhiteKing ? moveHashes[1][6] : 0;
	fullKey ^= _virginBlackKing ? moveHashes[2][6] : 0;
	return fullKey;
}

void ChessBoard::setupPieceLists() {
	for (uint8_t i = 0; i < 8; ++i) {
		for (uint8_t j = 0; j < 8; ++j) {
			int color = _squares[i][j] & COLORMASK;
			pieceCoords thisCoord = { i,j };
			int pListIndex = -1;
			if (_squares[i][j] != EMPTY) {
				int pieceTypeIndex = getIndexFromChar(_squares[i][j]);
				pListIndex = _pieceList[pieceTypeIndex].size();
				_uniqueIndex[i][j] = pListIndex;
				_pieceList[pieceTypeIndex].push_back(thisCoord);
			}
			
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////     MOVE GENERATION SECTION     /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ChessBoard::makeMove(MoveStr move) {
	// Actually make the move
	_squares[move.iEnd][move.jEnd] = move.movingPiece;
	_squares[move.iStart][move.jStart] = EMPTY;

	// Get the index of the piecelist array
	int movingPieceTypeI = getIndexFromChar(move.movingPiece);
	// Update the position of the moving piece in the piecelist
	_pieceList[movingPieceTypeI][_uniqueIndex[move.iStart][move.jStart]] = { move.iEnd,move.jEnd }; // I believe problem is here, 1/30
	
	_colorToMove ^= WHITE; // switch the color to move

						   // Update the zobrist hash key and displaced pieces accordingly
	_zobristKey ^= moveHashes[0][6]; // Encode the color switch
	_zobristKey ^= moveHashes[move.iStart * 8 + move.jStart][movingPieceTypeI]; // remove the start position piece
	_zobristKey ^= moveHashes[move.iEnd * 8 + move.jEnd][movingPieceTypeI]; // add the end position piece


	if (move.capturedPiece) {
		// Get the type of piece index
		int capturePieceTypeI = getIndexFromChar(move.capturedPiece);
		// get the unique array index on the secondary board
		int ui = _uniqueIndex[move.iEnd][move.jEnd];
		_pieceList[capturePieceTypeI][ui].i = 127; // Nothing here
		////////////ARRRRRRRG

		_capturedPieces.push_back(move.capturedPiece); //Add to captured pieces, maybe remove this line
		_zobristKey ^= moveHashes[move.iEnd * 8 + move.jEnd][getIndexFromChar(move.capturedPiece)]; // Remove the displacedPiece
	}
	_moveHistory.push_back(move);

}



void ChessBoard::unMakeMove(MoveStr move) {
	_squares[move.iStart][move.jStart] = move.movingPiece;
	_squares[move.iEnd][move.jEnd] = move.capturedPiece;

	_colorToMove ^= WHITE; // switch the color to move

						   // Update the zobrist hash key and displaced pieces accordingly
	_zobristKey ^= moveHashes[0][6]; // Encode the color switch
	_zobristKey ^= moveHashes[move.iStart * 8 + move.jStart][getIndexFromChar(move.movingPiece)]; // add back the moved piece to start
	_zobristKey ^= moveHashes[move.iEnd * 8 + move.jEnd][getIndexFromChar(move.movingPiece)]; // remove the moved piece from the end


	if (move.capturedPiece) {
		_capturedPieces.pop_back();
		_zobristKey ^= moveHashes[move.iEnd * 8 + move.jEnd][getIndexFromChar(move.capturedPiece)]; // Add back the displacedPiece
	}
	_moveHistory.pop_back();
}

bool ChessBoard::inBounds(int i, int j) {
	return (i >= 0 && i<8 && j >= 0 && j<8);
}


vector<MoveStr> ChessBoard::generatePossibleMoves() {
	vector<MoveStr> moveList;

	for (_i = 0; _i<8; ++_i) {
		for (_j = 0; _j<8; ++_j) {
			if ((_squares[_i][_j] & COLORMASK) == _colorToMove) { //This line is slowing us down
				switch (_squares[_i][_j] & PIECEMASK) {
				case PAWN: addPawnMoves(&moveList); break;
				case ROOK: addRookMoves(&moveList); break;
				case KNIGHT: addKnightMoves(&moveList); break;
				case BISHOP: addBishopMoves(&moveList); break;
				case QUEEN: addQueenMoves(&moveList); break;
				case KING: addKingMoves(&moveList); break;
				}
			}
		}
	}
	return moveList;
}


void ChessBoard::addMoveToList(uint8_t iEnd, uint8_t jEnd, int movingPiece, vector<MoveStr>* moveList) {
	if (moveList == NULL) return;
	MoveStr thisMove = { _i,_j,iEnd,jEnd,(uint8_t)movingPiece,(uint8_t)_squares[iEnd][jEnd] };
	moveList->emplace_back(thisMove); // I have a bad feeling about these uint8_t casts
}

void ChessBoard::addMoveWithPromotionsToList(uint8_t iEnd, uint8_t jEnd, int color, vector<MoveStr>* moveList) {
	// There is a bug where in terms of the zobrist hash, a promoted piece looks like a pawn
	if (moveList == NULL) return;

	uint8_t movPiece = PAWN | color;
	MoveStr thisMove = { _i,_j,iEnd,jEnd,movPiece,(uint8_t)_squares[iEnd][jEnd] };

	// Check for promotion option
	if ((iEnd == 7 && color == WHITE) || (iEnd == 0 && color == BLACK)) {

		int promotionChoices[4] = { QUEEN | color,ROOK | color,BISHOP | color,KNIGHT | color };
		for (int i = 0; i<4; ++i) {
			thisMove.movingPiece = (uint8_t)promotionChoices[i];
			moveList->emplace_back(thisMove);
		}

	}
	else moveList->emplace_back(thisMove);

}

int ChessBoard::addPawnMoves(vector<MoveStr>* moveList) {
	int color = _squares[_i][_j] & COLORMASK;
	int numMoves = 0; // number of pawn moves
	int di = color ? 1 : -1; // Advance is 1 for black and -1 for white
	int ddi = color ? 2 : -2; // double advance
	int dblAdvanceRow = color ? 1 : 6;
	// TODO add en passant, kernalize this code
	// Also remember the potential bug with numMoves != length(moveList) for promotions
	if (_squares[_i + di][_j] == EMPTY && inBounds(_i + di, _j)) {
		addMoveWithPromotionsToList(_i + di, _j, color, moveList); ++numMoves; //Single advance

		if ((_i == dblAdvanceRow) && (_squares[_i + ddi][_j] == EMPTY))
			addMoveWithPromotionsToList(_i + ddi, _j, color, moveList); ++numMoves; //Double advance
	}
	if (inBounds(_i + di, _j + 1) && (_squares[_i + di][_j + 1] != EMPTY) && ((_squares[_i + di][_j + 1] & COLORMASK) != color)) {
		addMoveWithPromotionsToList(_i + di, _j + 1, color, moveList); ++numMoves; //Capture right
	}
	if (inBounds(_i + di, _j - 1) && (_squares[_i + di][_j - 1] != EMPTY) && ((_squares[_i + di][_j - 1] & COLORMASK) != color)) {
		addMoveWithPromotionsToList(_i + di, _j - 1, color, moveList); ++numMoves; //Capture left
	}
	return numMoves;
}

int ChessBoard::addRookMoves(vector<MoveStr>* moveList) {
	int color = _squares[_i][_j] & COLORMASK;
	int numMoves = 0; // number of rook moves
	int movingPiece = ROOK | color;
	int di[4] = { 1,-1,0,0 }; // Move Kernel
	int dj[4] = { 0,0,1,-1 };
	for (int k = 0; k<4; ++k) {
		for (int ii = _i + di[k], jj = _j + dj[k]; inBounds(ii, jj); ii += di[k], jj += dj[k]) {
			bool open = _squares[ii][jj] == EMPTY;
			bool enemy = (_squares[ii][jj] & COLORMASK) != color;
			if (open || enemy) addMoveToList(ii, jj, movingPiece, moveList); ++numMoves;
			if (!open) break;
		}
	}
	return numMoves;
}

int ChessBoard::addKnightMoves(vector<MoveStr>* moveList) {
	int color = _squares[_i][_j] & COLORMASK;
	int numMoves = 0;
	int movingPiece = KNIGHT | color;

	for (int8_t oi = -1; oi <= 1; oi += 2) {
		for (int8_t oj = -1; oj <= 1; oj += 2) {
			if (inBounds(_i + 2 * oi, _j + oj) && (_squares[_i + 2 * oi][_j + oj] == EMPTY || (_squares[_i + 2 * oi][_j + oj] & COLORMASK) != color)) {
				addMoveToList(_i + 2 * oi, _j + oj, movingPiece, moveList); ++numMoves;
			}
			if (inBounds(_i + oi, _j + 2 * oj) && (_squares[_i + oi][_j + 2 * oj] == EMPTY || (_squares[_i + oi][_j + 2 * oj] & COLORMASK) != color)) {
				addMoveToList(_i + oi, _j + 2 * oj, movingPiece, moveList); ++numMoves;
			}
		}
	}
	return numMoves;
}

int ChessBoard::addBishopMoves(vector<MoveStr>* moveList) {
	int color = _squares[_i][_j] & COLORMASK;
	int numMoves = 0;
	int movingPiece = BISHOP | color;

	int di[4] = { 1,1,-1,-1 }; // Move Kernel
	int dj[4] = { 1,-1,1,-1 };
	for (int k = 0; k<4; ++k) {
		for (int ii = _i + di[k], jj = _j + dj[k]; inBounds(ii, jj); ii += di[k], jj += dj[k]) {
			bool open = _squares[ii][jj] == EMPTY;
			bool enemy = (_squares[ii][jj] & COLORMASK) != color;
			if (open || enemy) addMoveToList(ii, jj, movingPiece, moveList); ++numMoves;
			if (!open) break;
		}
	}
	return numMoves;
}

int ChessBoard::addQueenMoves(vector<MoveStr>* moveList) {
	int color = _squares[_i][_j] & COLORMASK;
	int numMoves = 0;
	int movingPiece = QUEEN | color;

	int di[8] = { 1,1,-1,-1,1,-1,0,0 }; // Move Kernel
	int dj[8] = { 1,-1,1,-1,0,0,-1,-1 };
	for (int k = 0; k<8; ++k) {
		for (int ii = _i + di[k], jj = _j + dj[k]; inBounds(ii, jj); ii += di[k], jj += dj[k]) {
			bool open = _squares[ii][jj] == EMPTY;
			bool enemy = (_squares[ii][jj] & COLORMASK) != color;
			if (open || enemy) addMoveToList(ii, jj, movingPiece, moveList); ++numMoves;
			if (!open) break;
		}
	}
	return numMoves;
}

int ChessBoard::addKingMoves(vector<MoveStr>* moveList) {
	int color = _squares[_i][_j] & COLORMASK;
	int numMoves = 0;
	int movingPiece = KING | color;
	int di[8] = { 1,1, 1, 0, 0, -1,-1,-1 }; // Move Kernel
	int dj[8] = { 1,0,-1, 1,-1,  1, 0,-1 };
	// TODO add castling
	for (int k = 0; k<8; ++k) {
		int8_t ii = _i + di[k], jj = _j + dj[k];
		bool open = _squares[ii][jj] == EMPTY;
		bool enemy = (_squares[ii][jj] & COLORMASK) != color;
		if (inBounds(ii, jj) && (open || enemy)) addMoveToList(ii, jj, movingPiece, moveList); ++numMoves;
	}
	return numMoves;
}
