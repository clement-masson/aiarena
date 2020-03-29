#include <iostream>
#include <stdlib.h>        // abs
#include <list>
#include <cassert>
#include "CGameState.h"

namespace Chess {

CGameState::CGameState() {
	cells = std::vector<CCell>(64);
	initBoard();
	isWhiteTurn = true;
  turnCounter = 0;
	noPawnNoCapture = 0; // 50 moves rule

    // en passant
	pawn_pushed_by_two = false;
	pawn_pushed_col = -1;

    // castle
	white_king_castle_A_side = true;
	white_king_castle_H_side = true;
	black_king_castle_A_side = true;
	black_king_castle_H_side = true;
}

void CGameState::initBoard(){
	cells[0] = CCell(PieceType::rook, true);
	cells[1] = CCell(PieceType::knight, true);
	cells[2] = CCell(PieceType::bishop, true);
	cells[3] = CCell(PieceType::queen, true);
	cells[4] = CCell(PieceType::king, true);
	cells[5] = CCell(PieceType::bishop, true);
	cells[6] = CCell(PieceType::knight, true);
	cells[7] = CCell(PieceType::rook, true);
	for (int cellIndex = 8; cellIndex < 16; cellIndex++)
		cells[cellIndex] = CCell(PieceType::pawn, true);
	for (int cellIndex = 48; cellIndex < 56; cellIndex++)
		cells[cellIndex] = CCell(PieceType::pawn, false);
	cells[56] = CCell(PieceType::rook, false);
	cells[57] = CCell(PieceType::knight, false);
	cells[58] = CCell(PieceType::bishop, false);
	cells[59] = CCell(PieceType::queen, false);
	cells[60] = CCell(PieceType::king, false);
	cells[61] = CCell(PieceType::bishop, false);
	cells[62] = CCell(PieceType::knight, false);
	cells[63] = CCell(PieceType::rook, false);
}


void CGameState::reverse(){
	isWhiteTurn = !isWhiteTurn;

	// Inversion des cellules
	CCell x;
	for (int cellIndex = 0; cellIndex < NCELLS/2; cellIndex++) {
		x = cells[cellIndex];
		cells[cellIndex] = cells[NCELLS-cellIndex-1].invertColor();
		cells[NCELLS-cellIndex-1] = x.invertColor();
	}

	// Inversion des infos de prise en passant
	if(pawn_pushed_by_two)
		pawn_pushed_col = NCOLUMNS - 1 - pawn_pushed_col;

	// inversion des infos de roque
	bool tmp;
	tmp = white_king_castle_A_side;
	white_king_castle_A_side = black_king_castle_H_side;
	black_king_castle_H_side = tmp;
	tmp = white_king_castle_H_side;
	white_king_castle_H_side = black_king_castle_A_side;
	black_king_castle_A_side = tmp;
}

std::string CGameState::getFEN(bool turn, bool castle, bool counts){
  std::string result = "";
	CCell cell;
  int empty_counter;
	for(int row = NROWS-1; row>=0; --row) {
    empty_counter = 0;
  	for(int column = 0; column<NCOLUMNS; ++column) {
  		cell = getCell(row, column);
      if (cell.pieceType == PieceType::none){
        empty_counter ++;
      }else{
        if (empty_counter>0){
          result += std::to_string(empty_counter);
        }
        empty_counter = 0;
        result += cell.toString();
      }
    }
    if (empty_counter>0){
      result += std::to_string(empty_counter);
    }
    if (row>0){
      result += "/";
    }
  }

  if(turn)
    result += isWhiteTurn ? " w" : " b";

  if(castle){
    result += " ";
    if (!(white_king_castle_A_side || white_king_castle_H_side || black_king_castle_A_side || black_king_castle_H_side)){
      result += "-";
    }else{
      if (white_king_castle_H_side)
        result += "K";
      if (white_king_castle_A_side)
        result += "Q";
      if (black_king_castle_H_side)
        result += "k";
      if (black_king_castle_A_side)
        result += "q";
    }
  }

  result += " ";
  if (pawn_pushed_by_two){
      result += std::to_string(pawn_pushed_col);
  }else{
      result += "-";
  }

  if(counts){
      result += " ";
      result += std::to_string(noPawnNoCapture);
      result += " ";
      result += std::to_string(turnCounter/2 + 1);
  }
	return result;
}

/*
   Converters, checkers and getters
 */

bool CGameState::isValidIndex(const int cellIndex){
	return (0<=cellIndex && cellIndex<NCELLS);
}

bool CGameState::isValidRC(const int r, const int c){
	return (0<=r && r<NROWS) && (0<=c && c<NCOLUMNS);
}

int CGameState::RCtoIndex(const int r, const int c){
	return r*NCOLUMNS + c;
}

std::pair<int,int> CGameState::indexToRC(const int cellIndex){
	return std::pair<int,int>(cellIndex / NCOLUMNS, cellIndex % NCOLUMNS);
}

CCell CGameState::getCell(const int cellIndex){
	if(!isValidIndex(cellIndex)) {
		std::cout << "Non valid index : " << cellIndex << "\n";
		throw "Non valid index";
	}
	return cells[cellIndex];
}

CCell CGameState::getCell(const int r, const int c){
	if(!isValidRC(r,c)) {
		std::cout << "Non valid coordinates : " << r << ", " << c << "\n";
		throw "Non valid coordinates";
	}
	return cells[RCtoIndex(r,c)];
}

void CGameState::setCell(const int cellIndex, const CCell c){
	if(!isValidIndex(cellIndex)) throw "Non valid coordinates in CGameState::setCell";
	cells[cellIndex] = c;
}

void CGameState::setCell(const int r, const int c, const CCell cell){
	if(!isValidRC(r,c)) throw "Non valid coordinates in CGameState::setCell";
	cells[RCtoIndex(r,c)] = cell;
}


std::vector<CMove*> CGameState::getPawnMovesFrom(const int cellIndex, const bool isWhite){
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
	CCell candidate;
	for(int directionCol = -1; directionCol<=1; directionCol+=2) {
		if (isValidRC(r + directionRow, c + directionCol)) {
			candidate = getCell(r + directionRow, c + directionCol);
			if (candidate.pieceType != PieceType::none && candidate.isWhite != isWhite) {

				// si on arrive sur la derniere rangée
				if (r + directionRow == 0 || r + directionRow == 7) {
					possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(r + directionRow, c + directionCol), true, PieceType::rook));
					possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(r + directionRow, c + directionCol), true, PieceType::knight));
					possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(r + directionRow, c + directionCol), true, PieceType::bishop));
					possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(r + directionRow, c + directionCol), true, PieceType::queen));
				}else{
					possibleMoves.push_back( new CMove(cellIndex, RCtoIndex(r + directionRow, c + directionCol), true));
				}
			}
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

std::vector<CMove*> CGameState::getRookMovesFrom(const int cellIndex, const bool isWhite){
	std::vector<CMove*> possibleMoves;

	std::pair<int,int> rc = indexToRC(cellIndex);
	int r = rc.first;
	int c = rc.second;
	int rr, cc;
	CCell dest;

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

std::vector<CMove*> CGameState::getBishopMovesFrom(const int cellIndex, const bool isWhite){
	std::vector<CMove*> possibleMoves;

	std::pair<int,int> rc = indexToRC(cellIndex);
	int r = rc.first;
	int c = rc.second;
	int rr, cc;
	CCell dest;

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


std::vector<CMove*> CGameState::getQueenMovesFrom(const int cellIndex, const bool isWhite){
	std::vector<CMove*> rookMoves = getRookMovesFrom(cellIndex, isWhite);
	std::vector<CMove*> bishopMoves = getBishopMovesFrom(cellIndex, isWhite);
	std::vector<CMove*> possibleMoves;
	possibleMoves.insert(possibleMoves.end(), rookMoves.begin(), rookMoves.end());
	possibleMoves.insert(possibleMoves.end(), bishopMoves.begin(), bishopMoves.end());
	return possibleMoves;
}

std::vector<CMove*> CGameState::getKnightMovesFrom(const int cellIndex, const bool isWhite){
	std::vector<CMove*> possibleMoves;

	std::pair<int,int> rc = indexToRC(cellIndex);
	int r = rc.first;
	int c = rc.second;
	int rr, cc;
	CCell dest;

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

std::vector<CMove*> CGameState::getKingMovesFrom(const int cellIndex, const bool isWhite){
	std::vector<CMove*> possibleMoves;

	std::pair<int,int> rc = indexToRC(cellIndex);
	int r = rc.first;
	int c = rc.second;
	int rr, cc;
	CCell dest;

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
	// std::cout << "KING MOVES OK" << std::endl;
	return possibleMoves;
}


std::vector<CMove*> CGameState::getCastleMoves(const int kingPosition, const bool whiteKing){
	std::vector<CMove*> possibleMoves;

	bool try_Aside = whiteKing ? white_king_castle_A_side : black_king_castle_A_side;
	bool try_Hside = whiteKing ? white_king_castle_H_side : black_king_castle_H_side;

	// Si le roi ou les deux tours ont bougés, alors on sait deja qu'on ne peut pas roquer
	if (!try_Aside && !try_Hside)
		return possibleMoves;

	std::pair<int,int> rc = indexToRC(kingPosition);
	int king_row = rc.first;
	int king_col = rc.second;
	assert((whiteKing && king_row == 0) || (king_row == NROWS - 1));
	assert(king_col == 3 || king_col == 4);                 // col may not be equal to 4 because of state reversing

	if (try_Aside) {
		for(int col = 1; col<king_col; ++col) {
			try_Aside &= getCell(king_row, col).pieceType == PieceType::none;
		}
	}

	if (try_Hside) {
		for(int col = king_col+1; col<7; ++col) {
			try_Hside &= getCell(king_row, col).pieceType == PieceType::none;
		}
	}

	// Si il y a des pieces entre le roi et les deux tours, on ne pourra pas roquer
	if (!try_Aside && !try_Hside)
		return possibleMoves;

	// si le roi est en echec, il est interdit de roquer
	if (isInCheck(kingPosition, whiteKing))
		return possibleMoves;

	if (try_Aside) {
		for(int col = king_col-2; col<=king_col-1; ++col) {
			if(isInCheck(RCtoIndex(king_row, col), whiteKing)) {
				try_Aside = false;
				break;
			}
		}
	}

	if (try_Hside) {
		for(int col = king_col+1; col<=king_col+2; ++col) {
			if(isInCheck(RCtoIndex(king_row, col), whiteKing)) {
				try_Hside = false;
				break;
			}
		}
	}

	// Si le roi n'est en echec sur aucune case intermediaire, coté A
	if (try_Aside) {
		possibleMoves.push_back( new CMove(kingPosition, RCtoIndex(king_row, king_col-2),
		                                   false, PieceType::none, false, -1));
	}
	// Si le roi n'est en echec sur aucune case intermediaire, coté H
	if (try_Hside) {
		possibleMoves.push_back( new CMove(kingPosition, RCtoIndex(king_row, king_col+2),
		                                   false, PieceType::none, false, +1));
	}

	return possibleMoves;
}

std::vector<CMove*> CGameState::findPossibleMoves(){
	return findPossibleMoves(isWhiteTurn);
}

std::vector<CMove*> CGameState::findPossibleMoves(const bool white){
	//     Find valid moves and their corresponding states for a given player.
	//
	//     Input:
	//         - color: the color of the player we want to find moves
	//     Outputs:
	//         - moves: the list containing very authhorized move.

	std::vector<CMove*> moves;
	// First we look for capturing moves (we are obliged to capture as many pieces as possible)
	std::vector<CMove*> pieceMoves;
	CGameState stateCopy;
	CCell piece;
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
        	std::vector<CMove*> castleMoves = getCastleMoves(cellIndex, white);
        	pieceMoves.insert(pieceMoves.end(), castleMoves.begin(), castleMoves.end());
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

bool CGameState::isInCheck(){
    return isInCheck(isWhiteTurn);
}

bool CGameState::isInCheck(const bool whiteKing){
	// Recherche de la position du roi
	CCell dest;
	int kingPosition = -1;
	for(int cellIndex = 0; cellIndex<NCELLS; cellIndex++) {
		dest = cells[cellIndex];
		if(dest.pieceType == PieceType::king &&  dest.isWhite == whiteKing) {
			kingPosition = cellIndex;
			break;
		}
	}
	assert(kingPosition != -1);
	return isInCheck(kingPosition, whiteKing);
}

bool CGameState::isInCheck(const int position, const bool whiteKing){
	std::vector<CMove*> pieceMoves;
	CCell dest;
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

	// King
	pieceMoves = getKingMovesFrom(position, whiteKing);
	for (std::vector<CMove*>::iterator it = pieceMoves.begin(); it != pieceMoves.end(); ++it) {
		move = *it;
		dest = cells[move->to_index];
		if(dest.pieceType == PieceType::king)
			return true;
	}

	// Pawn
	std::pair<int,int> rc = indexToRC(position);
	int r = rc.first;
	int c = rc.second;
	int directionRow = whiteKing ? 1 : -1;
	for(int directionCol = -1; directionCol<=1; directionCol+=2) {
		if(!isValidRC(r + directionRow, c + directionCol))
			continue;
		dest = getCell(r + directionRow, c + directionCol);
		if (dest.isWhite != whiteKing && dest.pieceType == PieceType::pawn)
			return true;
	}

	return false;
}


void CGameState::doMove(const CMove& move){
	/* Update the state according to the specified move

	   Note that this function does not check if the move is valid*/
	// std::cout << "CGameState : domove " << move.toString() << std::endl;
	if (!isValidIndex(move.from_index) or !isValidIndex(move.to_index)) {throw "Invalid Index !";}
	CCell piece = cells[move.from_index];
	std::pair<int,int> rc = indexToRC(move.from_index);
	int from_row = rc.first;
	int from_column = rc.second;
	rc = indexToRC(move.to_index);
	int to_row = rc.first;
	int to_column = rc.second;

	bool enpassant = move.isCapture && cells[move.to_index].pieceType == PieceType::none;
	bool roque = cells[move.from_index].pieceType == PieceType::king &&
	             abs(move.from_index - move.to_index) == 2;

	// Mise a jour des cellules
	cells[move.from_index] = CCell(PieceType::none);
	if(move.promotionType !=  PieceType::none) {
		cells[move.to_index] = CCell(move.promotionType, piece.isWhite);
	}else{
		cells[move.to_index] = piece;
	}

	// Gerer le cas en passant
	if(enpassant) {
		setCell(from_row, to_column, CCell(PieceType::none));
	}

	// Gerer le roque
	if(roque) {
		setCell((move.from_index + move.to_index)/2, CCell(PieceType::rook, piece.isWhite));
		if(from_column < to_column)
			setCell(from_row, NCOLUMNS-1, CCell(PieceType::none));
		else
			setCell(from_row, 0, CCell(PieceType::none));
	}

	// Mise a jour des info de 50moves
    turnCounter ++;
	if(piece.pieceType == PieceType::pawn || move.isCapture) {
		noPawnNoCapture = 0;                  // 50 moves rule
	}else{
		noPawnNoCapture++;
	}
	isWhiteTurn = !isWhiteTurn;

	// mettre à jour pawn_pushed_col
	if(piece.pieceType == PieceType::pawn && abs(from_row - to_row) == 2) {
		pawn_pushed_by_two = true;
		pawn_pushed_col = move.to_index % NCOLUMNS;
	}else{
		pawn_pushed_by_two = false;
		pawn_pushed_col = -1;
	}

	// mettre à jour les infos de roque
	if(piece.pieceType == PieceType::king && piece.isWhite) {
		white_king_castle_A_side = false;
		white_king_castle_H_side = false;
	}else if(piece.pieceType == PieceType::king && !piece.isWhite) {
		black_king_castle_A_side = false;
		black_king_castle_H_side = false;
	}else if(piece.pieceType == PieceType::rook && piece.isWhite && from_column == 0)
		white_king_castle_A_side = false;
	else if(piece.pieceType == PieceType::rook && piece.isWhite && from_column == NCOLUMNS-1)
		white_king_castle_H_side = false;
	else if(piece.pieceType == PieceType::rook && !piece.isWhite && from_column == 0)
		black_king_castle_A_side = false;
	else if(piece.pieceType == PieceType::rook && !piece.isWhite && from_column == NCOLUMNS-1)
		black_king_castle_H_side = false;

	// std::cout << "CGameState : domove " << move.toString() << " SUCCESSFUL" << std::endl;
}

}
