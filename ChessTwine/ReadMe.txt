========================================================================
    CONSOLE APPLICATION : ChessAI Project Overview
========================================================================

AppWizard has created this ChessAI application for you.

This file contains a summary of what you will find in each of the files that
make up your ChessAI application.


ChessAI.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

ChessAI.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

ChessAI.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named ChessAI.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////

			if (color == WHITE) {
				switch (_squares[i][j] & PIECEMASK) {
				case PAWN: pListIndex = _pieceList.wPawns.size;
					_pieceList.wPawns.push_back(thisCoord); break;
				case ROOK: pListIndex = _pieceList.wRooks.size; 
					_pieceList.wRooks.push_back(thisCoord); break;
				case KNIGHT: pListIndex = _pieceList.wKnights.size; 
					_pieceList.wKnights.push_back(thisCoord); break;
				case BISHOP: pListIndex = _pieceList.wBishops.size; 
					_pieceList.wBishops.push_back(thisCoord); break;
				case QUEEN: pListIndex = _pieceList.wQueens.size; 
					_pieceList.wQueens.push_back(thisCoord); break;
				case KING: pListIndex = _pieceList.wKings.size; 
					_pieceList.wKings.push_back(thisCoord); break;
				}
			} else {
				switch (_squares[i][j] & PIECEMASK) {
				case PAWN: pListIndex = _pieceList.bPawns.size; 
					_pieceList.bPawns.push_back(thisCoord); break;
				case ROOK: pListIndex = _pieceList.bRooks.size; 
					_pieceList.bRooks.push_back(thisCoord); break;
				case KNIGHT: pListIndex = _pieceList.bKnights.size; 
					_pieceList.bKnights.push_back(thisCoord); break;
				case BISHOP: pListIndex = _pieceList.bBishops.size; 
					_pieceList.bBishops.push_back(thisCoord); break;
				case QUEEN:  pListIndex = _pieceList.bQueens.size; 
					_pieceList.bQueens.push_back(thisCoord); break;
				case KING: pListIndex = _pieceList.bKings.size; 
					_pieceList.bKings.push_back(thisCoord); break;
				}
			}