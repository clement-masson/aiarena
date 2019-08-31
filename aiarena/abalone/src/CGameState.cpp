#include <iostream>
#include <stdlib.h>         // abs
#include <algorithm>        // max et min
#include <cassert>
#include "CGameState.h"

namespace Abalone {

CGameState::CGameState(int s) {
	size = s;
	cells = std::vector<CCell>(3*s*(s-1) + 1);
	isWhiteTurn = true;
	capturedWhiteBalls = 0;
	capturedBlackBalls = 0;
	turnCounter = 0;

	// initialisation des tableaux de conversion
	int index = 0;
	_IndexToRC = std::vector<std::pair<int,int> >(cells.size());
	for(int row=0; row<2*size-1; row++) {
		_RCtoIndex.push_back(std::vector<int>(2*s-1));
		for(int col=0; col<2*size-1; col++) {
			if(col < getRowStart(row) || col > getRowEnd(row)) {
				_RCtoIndex[row][col] = -1;
				continue;
			}
			_IndexToRC[index] = std::pair<int,int>(row, col);
			_RCtoIndex[row][col] = index;
			index++;
		}
	}
	assert(index==cells.size());
	initBoard();
}

int CGameState::getWidth(int row){
	return 2*size - 1 - abs(size-1 - row);
}

int CGameState::getRowStart(int row){
	return std::max(0, row - size + 1);
}

int CGameState::getRowEnd(int row){
	return size - 1 + std::min(size-1, row);
}

void CGameState::initBoard(){
	for (int row = 0; row <= 2; row++) {
		for (int col = getRowStart(row) + (row==2 ? 2 : 0);
		     col <= getRowEnd(row) - (row==2 ? 2 : 0); col++) {
			setCell(row, col, CCell(CCell::WHITE));
		}
	}
	for (int row = 2*size-4; row <= 2*size-2; row++) {
		for (int col = getRowStart(row) + (row==2*size-4 ? 2 : 0);
		     col <= getRowEnd(row) - (row==2*size-4 ? 2 : 0); col++) {
			setCell(row, col, CCell(CCell::BLACK));
		}
	}
}


void CGameState::reverse(){
	isWhiteTurn = !isWhiteTurn;
	int ncells = cells.size();

    // inversion des captures
    int temp = capturedWhiteBalls;
    capturedWhiteBalls = capturedBlackBalls;
    capturedBlackBalls = temp;

	// Inversion des cellules
	CCell x;
	for (int cellIndex = 0; cellIndex < (ncells)/2; cellIndex++) {
		x = cells[cellIndex];
		cells[cellIndex] = cells[ncells-cellIndex-1].invertColor();
		cells[ncells-cellIndex-1] = x.invertColor();
	}
	cells[(ncells-1)/2] = cells[(ncells-1)/2].invertColor();
}

/*
   Converters, getters and setters
 */

bool CGameState::isValidIndex(const int cellIndex){
	return (0<=cellIndex && cellIndex<cells.size());
}

bool CGameState::isValidRC(const int r, const int c){
	return (0<=r && r<2*size-1) && (0<=c && c<2*size-1) && abs(r-c)<size;
}

int CGameState::RCtoIndex(const int r, const int c){
	assert(isValidRC(r, c));
	return _RCtoIndex[r][c];
}

std::pair<int,int> CGameState::indexToRC(const int cellIndex){
	assert(isValidIndex(cellIndex));
	return _IndexToRC[cellIndex];
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

std::vector<CCell> CGameState::getRow(int row){
    std::vector<CCell> result;
    int c = getRowStart(row);
    for(int x=0; x<getWidth(row); x++){
        result.push_back(getCell(row, c+x));
    }
    return result;
}

std::vector<CCell> CGameState::getColumn(int col){
    std::vector<CCell> result;
    int r = getRowStart(col);
    for(int x=0; x<getWidth(col); x++){
        result.push_back(getCell(r+x, col));
    }
    return result;
}

std::vector<CCell> CGameState::getDiagonal(int diag){
    std::vector<CCell> result;
    int r = getRowStart(diag);
    int c = getRowStart(2*size - 2 - diag);
    for(int x=0; x<getWidth(diag); x++){
        result.push_back(getCell(r+x, c+x));
    }
    return result;
}

std::vector<CMove*> CGameState::getPushMoves(int cellindex){
    std::vector<CMove*> all_moves;
    char color = cells[cellindex].color;
    if(color == CCell::NONE){
        return all_moves;
    }
    int dx;
    std::pair<int,int> rc = indexToRC(cellindex);
    int row = rc.first;
    int col = rc.second;
    int r,c;
    int samecolor, oppcolor;
    char color2;
    for(int drow=-1; drow<=1; drow++){
        for(int dcol=-1; dcol<=1; dcol++){
            if((dcol*drow==-1)|| (dcol==0 && drow==0)){
                continue;
            }
            // printf("\t_ %i,%i\n", drow, dcol);
            samecolor = 1;
            oppcolor = 0;
            for(int dx=1; true; dx++){
                r = row + drow*dx;
                c = col + dcol*dx;
                if(!isValidRC(r,c)){
                    if(samecolor>oppcolor && oppcolor>0){
                        all_moves.push_back(new CMove(cellindex, RCtoIndex(row+drow, col+dcol)));
                        // std::cout << all_moves[all_moves.size()-1]->toPDN()+"\n";
                    }
                    break;
                }
                color2 = getCell(r, c).color;
                if(color2 == CCell::NONE){
                    if(samecolor>oppcolor){
                        all_moves.push_back(new CMove(cellindex, RCtoIndex(row+drow, col+dcol)));
                        // std::cout << all_moves[all_moves.size()-1]->toPDN()+"\n";
                    }
                    break;
                }else if(color2 == color){
                    samecolor++;
                    if(samecolor>3 || oppcolor>0){
                        break;
                    }
                }else{
                    oppcolor++;
                    if(oppcolor>2){
                        break;
                    }
                }
            }
        }
    }
}


std::vector<CMove*> CGameState::findPossibleMoves(){
    char color = isWhiteTurn ? CCell::WHITE : CCell::BLACK;
	std::vector<CMove*> all_moves, moves;
	if(checkTermination() != 0) {
		return all_moves;
	}

    for(int cellindex=0; cellindex<cells.size(); cellindex++){
        if(cells[cellindex].color == color){
            // printf("Looking at moves from %i\n", cellindex);
            moves = getPushMoves(cellindex);
            all_moves.insert(all_moves.end(), moves.begin(), moves.end());
            // printf("# moves %i\n", all_moves.size());
        }
    }

	return all_moves;
}


void CGameState::doMove(const CMove& move){
	/* Update the state according to the specified move

	   Note that this function does not check if the move is valid*/
	// std::cout << "CGameState : domove " << move.toPDN() << std::endl;
	isWhiteTurn = !isWhiteTurn;
	turnCounter++;
    if(move.from_end<0){
        std::pair<int,int> rc_from = indexToRC(move.from_start);
        int row_from = rc_from.first;
        int col_from = rc_from.second;
        std::pair<int,int> rc_to = indexToRC(move.to_start);
        int drow = rc_to.first - row_from;
        int dcol = rc_to.second - col_from;

        int r,c;
        CCell prev = cells[move.from_start];
        CCell curr;
        setCell(move.from_start, CCell(CCell::NONE));
        for(int dx=1; true; dx++){
            r = row_from + drow*dx;
            c = col_from + dcol*dx;
            if(!isValidRC(r,c)){
                if(prev.color == CCell::WHITE){
                    capturedWhiteBalls++;
                }else if(prev.color == CCell::BLACK){
                    capturedBlackBalls++;
                }else{
                    throw "Erreur dans doMove";
                }
                break;
            }
            curr = getCell(r, c);
            setCell(r, c, prev);
            prev = curr;
            if(curr.color == CCell::NONE){
                break;
            }
        }
    }
	// std::cout << "CGameState : domove " << move.toPDN() << " SUCCESSFUL" << std::endl;
}

int CGameState::checkTermination(){
	/* Cette fonction renvoie :
	    - 0 si la partie n'est pas terminée
	    - 1 si le joueur 1 (blanc) gagne
	    - 2 si le joueur 2 (noir) gagne
	    - 3 si égalité
	   Le comportement n'est pas garanti si la fonction est appliquée sur un état
	   illégal (i.e si les deux joueurs ont chacun un nombre de billes capturées
       superieur au seuil de victoire)
	 */
    if(capturedWhiteBalls == VICTORY_CAPTURES){
        return 2;
    }else if(capturedBlackBalls == VICTORY_CAPTURES){
        return 1;
    }
    return 0;
}

std::string CGameState::toString(){
	std::string result = "";
	CCell cell;
	int empty_counter;
	for(int row = 2*size-2; row>=0; --row) {
		empty_counter = 0;
		for(int column = getRowStart(row); column<=getRowEnd(row); ++column) {
			cell = getCell(row, column);
			if (cell.color == CCell::NONE) {
				empty_counter++;
			}else{
				if (empty_counter>0) {
					result += std::to_string(empty_counter);
				}
				empty_counter = 0;
				result += cell.toString();
			}
		}
		if (empty_counter>0) {
			result += std::to_string(empty_counter);
		}
		if (row>0) {
			result += "/";
		}
	}
	result += isWhiteTurn ? " w" : " b";

    result += " " + std::to_string(capturedWhiteBalls) + ":" + std::to_string(capturedBlackBalls);
	return result;
}

}
