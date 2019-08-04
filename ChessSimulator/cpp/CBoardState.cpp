#include <iostream>
#include <stdlib.h>        // abs
#include <list>
#include <cassert>
#include "CBoardState.h"

namespace Chess {

CBoardState::CBoardState() {
	cells = std::vector<Cell>(64);
	initBoard();
    pawn_pushed_by_two = false;
    pawn_pushed_col = -1;
    white_A_rook_moved = false;
    white_H_rook_moved = false;
    white_king_moved = false;
    black_A_rook_moved = false;
    black_H_rook_moved = false;
    black_king_moved = false;
}

void CBoardState::initBoard(){
	cells[0] = Cell(PieceType::rook, true);
	cells[1] = Cell(PieceType::knight, true);
	cells[2] = Cell(PieceType::bishop, true);
	cells[3] = Cell(PieceType::queen, true);
	cells[4] = Cell(PieceType::king, true);
	cells[5] = Cell(PieceType::bishop, true);
	cells[6] = Cell(PieceType::knight, true);
	cells[7] = Cell(PieceType::rook, true);
	for (int cellIndex = 8; cellIndex < 16; cellIndex++)
		cells[cellIndex] = Cell(PieceType::pawn, true);
	for (int cellIndex = 48; cellIndex < 56; cellIndex++)
		cells[cellIndex] = Cell(PieceType::pawn, false);
	cells[56] = Cell(PieceType::rook, false);
	cells[57] = Cell(PieceType::knight, false);
	cells[58] = Cell(PieceType::bishop, false);
	cells[59] = Cell(PieceType::queen, false);
	cells[60] = Cell(PieceType::king, false);
	cells[61] = Cell(PieceType::bishop, false);
	cells[62] = Cell(PieceType::knight, false);
	cells[63] = Cell(PieceType::rook, false);
}


void CBoardState::reverse(){
	Cell x;
    bool tmp;
    // Inversion des cellules
	for (int cellIndex = 0; cellIndex < NCELLS/2; cellIndex++) {
		x = cells[cellIndex];
		cells[cellIndex] = cells[NCELLS-cellIndex-1].invertColor();
		cells[NCELLS-cellIndex-1] = x.invertColor();
	}
    // Inversion des infos de prise en passant
    if(pawn_pushed_by_two)
        pawn_pushed_col = NCOLUMNS - 1 - pawn_pushed_col;

    // inversion des infos de roque
    tmp = white_A_rook_moved;
    white_A_rook_moved = black_A_rook_moved;
    black_A_rook_moved = tmp;

    tmp = white_H_rook_moved;
    white_H_rook_moved = black_H_rook_moved;
    black_H_rook_moved = tmp;

    tmp = white_king_moved;
    white_king_moved = black_king_moved;
    black_king_moved = tmp;
}

// std::string CBoardState::toString(){
// 	return std::string(cells.begin(), cells.end());
// }

/*
   Converters, checkers and getters
 */

bool CBoardState::isValidIndex(const int cellIndex){
	return (0<=cellIndex && cellIndex<NCELLS);
}

bool CBoardState::isValidRC(const int r, const int c){
	return (0<=r && r<NROWS) && (0<=c && c<NCOLUMNS);
}

int CBoardState::RCtoIndex(const int r, const int c){
	return r*NCOLUMNS + c;
}

std::pair<int,int> CBoardState::indexToRC(const int cellIndex){
	return std::pair<int,int>(cellIndex / NCOLUMNS, cellIndex % NCOLUMNS);
}

Cell CBoardState::getCell(const int cellIndex){
	if(!isValidIndex(cellIndex)) {
		std::cout << "Non valid index : " << cellIndex << "\n";
		throw "Non valid index";
	}
	return cells[cellIndex];
}

Cell CBoardState::getCell(const int r, const int c){
	if(!isValidRC(r,c)) {
		std::cout << "Non valid coordinates : " << r << ", " << c << "\n";
		throw "Non valid coordinates";
	}
	return cells[RCtoIndex(r,c)];
}

void CBoardState::setCell(const int cellIndex, const Cell c){
	if(!isValidIndex(cellIndex)) throw "Non valid coordinates";
	cells[cellIndex] = c;
}

void CBoardState::setCell(const int r, const int c, const Cell cell){
	if(!isValidRC(r,c)) throw "Non valid coordinates";
	cells[RCtoIndex(r,c)] = cell;
}


std::vector<CMove*> CBoardState::getPawnMovesFrom(const int cellIndex, const bool isWhite){
	Cell cell = cells[cellIndex];
	assert(cell.pieceType == PieceType::pawn && cell.isWhite == isWhite);
	std::vector<CMove*> possibleMoves;

	std::pair<int,int> rc = indexToRC(cellIndex);
	int r = rc.first;
	int c = rc.second;
	int directionRow = isWhite ? 1 : -1;

	// Avancee d'une case
	if ( isValidRC(r + directionRow, c) && getCell(r + directionRow, c).pieceType == PieceType::none) {
		// si la case devant le pion est libre
		// si on arrive sur la derniere rangée
		if (r + directionRow == 0 || r + directionRow == 7) {
			possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(r + directionRow, c), false, PieceType::rook));
			possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(r + directionRow, c), false, PieceType::knight));
			possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(r + directionRow, c), false, PieceType::bishop));
			possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(r + directionRow, c), false, PieceType::queen));
		}else{
			// sinon
			possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(r + directionRow, c)));
			// si c'est le 1er coup du pion et que les 2 case devant sont libres
			if ( (isWhite && r == 1 && getCell(3, c).pieceType == PieceType::none) ||
			     (!isWhite && r == 6 && getCell(4, c).pieceType == PieceType::none)) {
				possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(r + 2*directionRow, c)));
			}
		}
	}

	// Prise
    Cell candidate;
	for(int directionCol = -1; directionCol<=1; directionCol+=2) {
    	if (isValidRC(r + directionRow, c + directionCol)){
            candidate = getCell(r + directionRow, c + directionCol);
            if (candidate.pieceType != PieceType::none && candidate.isWhite != isWhite)
        		possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(r + directionRow, c + directionCol), true));
        }
    }

	// Prise en passant
    int rr, cc;
	if(pawn_pushed_by_two) {
		cc = pawn_pushed_col;
		rr = isWhite ? 4 : 3;
		if (r == rr && (c==cc-1 || c==cc+1))
			possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(r + directionRow, cc), true));
	}
	return possibleMoves;
}

std::vector<CMove*> CBoardState::getRookMovesFrom(const int cellIndex, const bool isWhite){
	Cell cell = cells[cellIndex];
	assert(cell.isWhite == isWhite && (cell.pieceType == PieceType::rook || cell.pieceType == PieceType::queen) );
	std::vector<CMove*> possibleMoves;

	std::pair<int,int> rc = indexToRC(cellIndex);
	int r = rc.first;
	int c = rc.second;
	int rr, cc;
	Cell dest;

	for(int horizontal = 0; horizontal<=1; horizontal++) {
		for(int direction = -1; direction<=1; direction+=2) {
			for(int d = 1; d<horizontal*NCOLUMNS + (1-horizontal)*NROWS; ++d) {
				rr = r + (1-horizontal) * direction * d;
				cc = c + horizontal * direction * d;
				if(!isValidRC(rr, cc))
					break;
				dest = getCell(rr, cc);
				if(dest.pieceType == PieceType::none) {
					possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(rr, cc)));
				}else{
					if(dest.isWhite != isWhite) {
						possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(rr, cc), true));
					}
					break;
				}
			}
		}
	}
	return possibleMoves;
}

std::vector<CMove*> CBoardState::getBishopMovesFrom(const int cellIndex, const bool isWhite){
	Cell cell = cells[cellIndex];
	assert(cell.isWhite == isWhite && (cell.pieceType == PieceType::bishop || cell.pieceType == PieceType::queen) );
	std::vector<CMove*> possibleMoves;

	std::pair<int,int> rc = indexToRC(cellIndex);
	int r = rc.first;
	int c = rc.second;
	int rr, cc;
	Cell dest;

	for(int directionRow = -1; directionRow<=1; directionRow+=2) {
		for(int directionCol = -1; directionCol<=1; directionCol+=2) {
			for(int d = 1; d<NCOLUMNS || d<NROWS; ++d) {
				rr = r + directionRow * d;
				cc = c + directionCol * d;
				if(!isValidRC(rr, cc))
					break;
				dest = getCell(rr, cc);
				if(dest.pieceType == PieceType::none) {
					possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(rr, cc)));
				}else{
					if(dest.isWhite != isWhite) {
						possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(rr, cc), true));
					}
					break;
				}
			}
		}
	}
	return possibleMoves;
}


std::vector<CMove*> CBoardState::getQueenMovesFrom(const int cellIndex, const bool isWhite){
	assert(cell.isWhite == isWhite && cell.pieceType == PieceType::queen);
	std::vector<CMove*> rookMoves = getRookMovesFrom(cellIndex, isWhite);
	std::vector<CMove*> bishopMoves = getBishopMovesFrom(cellIndex, isWhite);
	std::vector<CMove*> possibleMoves;
	possibleMoves.insert(possibleMoves.end(), rookMoves.begin(), rookMoves.end());
	possibleMoves.insert(possibleMoves.end(), bishopMoves.begin(), bishopMoves.end());
	return possibleMoves;
}

std::vector<CMove*> CBoardState::getKnightMovesFrom(const int cellIndex, const bool isWhite){
	Cell cell = cells[cellIndex];
	assert(cell.isWhite == isWhite && cell.pieceType == PieceType::knight );
	std::vector<CMove*> possibleMoves;

	std::pair<int,int> rc = indexToRC(cellIndex);
	int r = rc.first;
	int c = rc.second;
	int rr, cc;
	Cell dest;

	for(int directionRow = -2; directionRow<=2; directionRow++) {
		for(int directionCol = -2; directionCol<=2; directionCol++) {
			if( abs(directionRow) + abs(directionCol) != 3)
				continue;
			rr = r + directionRow;
			cc = c + directionCol;
			if(!isValidRC(rr, cc))
				continue;
			dest = getCell(rr, cc);
			if(dest.pieceType == PieceType::none) {
				possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(rr, cc)));
			}else if(dest.isWhite != isWhite) {
				possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(rr, cc), true));
			}
		}
	}
	return possibleMoves;
}

std::vector<CMove*> CBoardState::getKingMovesFrom(const int cellIndex, const bool isWhite){
	Cell cell = cells[cellIndex];
	assert(cell.isWhite == isWhite && cell.pieceType == PieceType::king );
	std::vector<CMove*> possibleMoves;

	std::pair<int,int> rc = indexToRC(cellIndex);
	int r = rc.first;
	int c = rc.second;
	int rr, cc;
	Cell dest;

	for(int directionRow = -1; directionRow<=1; directionRow++) {
		for(int directionCol = -1; directionCol<=1; directionCol++) {
			if(directionRow==0 && directionCol==0)
				continue;
			rr = r + directionRow;
			cc = c + directionCol;
			if(!isValidRC(rr, cc))
				continue;
			dest = getCell(rr, cc);
			if(dest.pieceType == PieceType::none) {
				possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(rr, cc)));
			}else if(dest.isWhite != isWhite) {
				possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(rr, cc), true));
			}
		}
	}

	// TODO:  AJOUTER GESTION DU ROQUE
	return possibleMoves;
}


std::vector<CMove*> CBoardState::findPossibleMoves(const bool white){
	//     Find valid moves and their corresponding states for a given player.
	//
	//     Input:
	//         - color: the color of the player we want to find moves
	//     Outputs:
	//         - moves: the list containing very authhorized move.

	std::vector<CMove*> moves;
	// First we look for capturing moves (we are obliged to capture as many pieces as possible)
	std::vector<CMove*> pieceMoves;
	CBoardState stateCopy;
	Cell piece;
	CMove *move;
	for(int cellIndex = 0; cellIndex<NCELLS; ++cellIndex) {
		piece = cells[cellIndex];

		// std::cout << "Searching moves for "<< piece.pieceType << " at location " << cellIndex << std::endl;

		if(piece.pieceType == PieceType::pawn && piece.isWhite==white) {
			pieceMoves = getPawnMovesFrom(cellIndex, white);
		}else if(piece.pieceType == PieceType::rook && piece.isWhite==white) {
			pieceMoves = getRookMovesFrom(cellIndex, white);
		}else if(piece.pieceType == PieceType::knight && piece.isWhite==white) {
			pieceMoves = getKnightMovesFrom(cellIndex, white);
		}else if(piece.pieceType == PieceType::bishop && piece.isWhite==white) {
			pieceMoves = getBishopMovesFrom(cellIndex, white);
		}else if(piece.pieceType == PieceType::queen && piece.isWhite==white) {
			pieceMoves = getQueenMovesFrom(cellIndex, white);
		}else if(piece.pieceType == PieceType::king && piece.isWhite==white) {
			pieceMoves = getKingMovesFrom(cellIndex, white);
		}else{
			continue;
		}

		// std::cout << "Found "<< pieceMoves.size() << " possible moves" << std::endl;

		for (std::vector<CMove*>::iterator it = pieceMoves.begin(); it != pieceMoves.end(); ++it) {
			move = *it;
			stateCopy = *this;
			stateCopy.doMove(*move);
			if( !stateCopy.isInCheck(white))
				moves.push_back(move);
            else
                free(move);
		}
	}
	return moves;
}

bool CBoardState::isInCheck(const bool whiteKing){
    // Recherche de la position du roi
    Cell dest;
    int kingPosition = -1;
    for(int cellIndex = 0; cellIndex<NCELLS; cellIndex++) {
        dest = cells[cellIndex];
		if(dest.pieceType == PieceType::king &&  dest.isWhite == whiteKing){
            kingPosition = cellIndex;
            break;
        }
    }
    assert(kingPosition != -1);
    return isInCheck(kingPosition, whiteKing);
}

bool CBoardState::isInCheck(const int position, const bool whiteKing){
    std::vector<CMove*> pieceMoves;
    Cell dest;
	CMove *move;

    // Rook and Queen
    pieceMoves = getRookMovesFrom(position, whiteKing);
	for (std::vector<CMove*>::iterator it = pieceMoves.begin(); it != pieceMoves.end(); ++it) {
		move = *it;
        dest = cells[move->to_index];
		if(dest.pieceType == PieceType::rook ||  dest.pieceType == PieceType::queen)
			return true;
	}

    // Bishop and Queen
    pieceMoves = getBishopMovesFrom(position, whiteKing);
	for (std::vector<CMove*>::iterator it = pieceMoves.begin(); it != pieceMoves.end(); ++it) {
		move = *it;
        dest = cells[move->to_index];
		if(dest.pieceType == PieceType::bishop ||  dest.pieceType == PieceType::queen)
			return true;
	}

    // Knight
    pieceMoves = getKnightMovesFrom(position, whiteKing);
	for (std::vector<CMove*>::iterator it = pieceMoves.begin(); it != pieceMoves.end(); ++it) {
		move = *it;
        dest = cells[move->to_index];
		if(dest.pieceType == PieceType::knight)
			return true;
	}

    // Pawn
    std::pair<int,int> rc = indexToRC(position);
	int r = rc.first;
	int c = rc.second;
	int directionRow = whiteKing ? 1 : -1;
	for(int directionCol = -1; directionCol<=1; directionCol+=2) {
        dest = getCell(r + directionRow, c + directionCol);
        if (dest.isWhite != whiteKing && dest.pieceType == PieceType::pawn)
    		return true;
    }

	return false;
}


void CBoardState::doMove(const CMove& move){
	/* Update the state according to the specified move

	   Note that this function does not check if the move is valid*/
    // std::cout << "CBoardState : domove " << move.toPDN() << std::endl;
    if (!isValidIndex(move.from_index) or !isValidIndex(move.to_index)) {throw "Invalid Index !";}
	Cell piece = cells[move.from_index];
	std::pair<int,int> rc = indexToRC(move.from_index);
	int from_row = rc.first;
	int from_column = rc.second;
	rc = indexToRC(move.to_index);
	int to_row = rc.first;
	int to_column = rc.second;

    bool enpassant = move.isCapture && cells[move.to_index].pieceType == PieceType::none;

    // Mise a jour des cellules
	cells[move.from_index] = Cell(PieceType::none);
	if(move.promotionType !=  PieceType::none) {
		cells[move.to_index] = Cell(move.promotionType, piece.isWhite);
	}else{
		cells[move.to_index] = piece;
	}

    // Gerer le cas en passant
    if(enpassant){
        setCell(from_row, to_column, Cell(PieceType::none));
    }

	// mettre à jour pawn_pushed_col
    if(piece.pieceType == PieceType::pawn && abs(from_row - to_row) == 2){
        pawn_pushed_by_two = true;
        pawn_pushed_col = move.to_index % NCOLUMNS;
    }else{
        pawn_pushed_by_two = false;
        pawn_pushed_col = -1;
    }

	// mettre à jour les infos de roque
    if(piece.pieceType == PieceType::king && piece.isWhite)
        white_king_moved = false;
    else if(piece.pieceType == PieceType::king && ! piece.isWhite)
        black_king_moved = false;
    else if(piece.pieceType == PieceType::king && piece.isWhite && from_column == 0)
        white_A_rook_moved = false;
    else if(piece.pieceType == PieceType::king && piece.isWhite && from_column == NCOLUMNS-1)
        white_H_rook_moved = false;
    else if(piece.pieceType == PieceType::king && !piece.isWhite && from_column == 0)
        black_A_rook_moved = false;
    else if(piece.pieceType == PieceType::king && !piece.isWhite && from_column == NCOLUMNS-1)
        black_H_rook_moved = false;
}

}
