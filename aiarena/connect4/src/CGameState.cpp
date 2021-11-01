#include <iostream>
#include <stdlib.h>        // abs
#include <cassert>
#include "CGameState.h"

namespace Connect4 {

std::vector<std::string> splitString(const std::string & string, const std::string & sep){
	std::vector<std::string> result;
	std::string remaining = string;
	size_t sep_pos = remaining.find(sep);

	while(sep_pos != std::string::npos){
		result.push_back(remaining.substr(0, sep_pos));
		remaining = remaining.substr(sep_pos + sep.size());
		sep_pos = remaining.find(sep);
	}
	result.push_back(remaining);

	return result;
}

CGameState::CGameState(int w, int h) {
    width = w;
    height = h;
    cells = std::vector<CCell>(w*h);
    initBoard();
    isWhiteTurn = true;
    turnCounter = 0;
}

void CGameState::initBoard(){
}


void CGameState::reverse(){
	isWhiteTurn = !isWhiteTurn;
	// Inversion des cellules
	for (int cellIndex = 0; cellIndex < width * height; cellIndex++) {
		cells[cellIndex] = cells[cellIndex].invertColor();
	}
}

std::string CGameState::toString(){
    std::string result;
    CCell cell;
    for(int col = 0; col<width; ++col) {
        for(int row = 0; row<height; ++row) {
            cell = getCell(row, col);
            if (cell.color == CCell::NONE){
                break;
            }else{
                result += cell.toString();
            }
        }
        if (col < width-1){
            result += "/";
        }
    }

    result += isWhiteTurn ? " w" : " b";
	return result;
}

void CGameState::setCellsFromString(const std::string & repr){
	std::vector<std::string> substrings = splitString(repr, " ");
	if(substrings.size() != 2) throw std::runtime_error("Bad string formatting");

	unsigned int col = 0;
	unsigned int row = 0;
	for(const char & c : substrings[0]) {
        if(c == '/'){
			col++;
			row = 0;
		}else{
			setCell(row, col, CCell::fromChar(c));
			row++;
		}
	}
	if(col!=(width-1)) throw std::runtime_error("Bad board description");

	// info de tour
	if(substrings[1].size() != 1) throw std::runtime_error("Bad string formatting");
	isWhiteTurn = substrings[1] == "w";
}

/*
   Converters, getters and getters
 */

bool CGameState::isValidIndex(const int cellIndex){
	return (0<=cellIndex && cellIndex<width*height);
}

bool CGameState::isValidRC(const int r, const int c){
	return (0<=r && r<height) && (0<=c && c<width);
}

int CGameState::RCtoIndex(const int r, const int c){
	return r*width + c;
}

std::pair<int,int> CGameState::indexToRC(const int cellIndex){
	return std::pair<int,int>(cellIndex / width, cellIndex % width);
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


std::vector<CMove*> CGameState::findPossibleMoves(){
	std::vector<CMove*> moves;
    if(checkTermination() != 0){
        return moves;
    }
	for(int col = 0; col<width; ++col) {
    	for(int row = 0; row<height; ++row) {
            if (getCell(row, col).color == CCell::NONE){
                moves.push_back(new CMove(col));
                break;
            }
        }
    }
	return moves;
}


void CGameState::doMove(const CMove& move){
	/* Update the state according to the specified move

	   Note that this function does not check if the move is valid*/
	// std::cout << "CGameState : domove " << move.toString() << std::endl;
	for(int row = 0; row<height; ++row) {
        if (getCell(row, move.column).color == CCell::NONE){
            setCell(row, move.column, CCell( isWhiteTurn ? CCell::WHITE : CCell::BLACK));
            break;
        }
    }
	isWhiteTurn = !isWhiteTurn;
	// std::cout << "CGameState : domove " << move.toString() << " SUCCESSFUL" << std::endl;
}

int CGameState::checkTermination(){
    /* Cette fonction renvoie :
        - 0 si la partie n'est pas terminée
        - 1 si le joueur 1 (blanc) gagne
        - 2 si le joueur 2 (noir) gagne
        - 3 si égalité
    Le comportement n'est pas garanti si la fonction est appliquée sur un état
    illégal (i.e si les deux joueurs ont chacun un alignement de + de 4 pions)
    */
    int current_counter;
    char current_color;
    char c;
    int dx;
    int npieces = 0;

    // check les verticales + le nombre de pieces
    // printf("check les verticales\n");
	for(int col = 0; col<width; ++col) {
        current_counter = 0;
        current_color = CCell::NONE;
    	for(int row = 0; row<height; ++row) {
            c = getCell(row, col).color;
            if(c != CCell::NONE){
                npieces++;
            }
            if (c == current_color){
                current_counter++;
                if(current_counter >=4  && current_color != CCell::NONE){
                    return current_color == CCell::WHITE ? 1 : 2;
                }
            }else{
                current_counter = 1;
                current_color = c;
            }
        }
    }

    // check les horizontales
    // printf("check les horizontales\n");
    for(int row = 0; row<height; ++row) {
        current_counter = 0;
        current_color = CCell::NONE;
    	for(int col = 0; col<width; ++col) {
            c = getCell(row, col).color;
            if (c == current_color){
                current_counter++;
                if(current_counter >=4  && current_color != CCell::NONE){
                    return current_color == CCell::WHITE ? 1 : 2;
                }
            }else{
                current_counter = 1;
                current_color = c;
            }
        }
    }

    // check les diagonales bas gauche vers haut droit (+1, +1)
    // printf("check les diagonales 1\n");
    for(int row = height-4; row>=0; --row) {
        for(int col = 0; col<= (row>0 ? 0 : width-4); ++col) {
            current_counter = 1;
            current_color = getCell(row, col).color;
            dx = 1;
        	while(isValidRC(row + dx, col + dx)) {
                c = getCell(row + dx, col + dx).color;
                if (c == current_color){
                    current_counter++;
                    if(current_counter >=4  && current_color != CCell::NONE){
                        return current_color == CCell::WHITE ? 1 : 2;
                    }
                }else{
                    current_counter = 1;
                    current_color = c;
                }
                dx++;
            }
        }
    }

    // check les diagonales haut gauche vers bas droit (-1, +1)
    // printf("check les diagonales 2\n");
    for(int row = 3; row<height; ++row) {
        for(int col = 0; col<= (row<height-1 ? 0 : width-4); ++col) {
            current_counter = 1;
            current_color = getCell(row, col).color;
            dx = 1;
        	while(isValidRC(row - dx, col + dx)) {
                c = getCell(row - dx, col + dx).color;
                if (c == current_color){
                    current_counter++;
                    if(current_counter >=4  && current_color != CCell::NONE){
                        return current_color == CCell::WHITE ? 1 : 2;
                    }
                }else{
                    current_counter = 1;
                    current_color = c;
                }
                dx++;
            }
        }
    }
    return npieces == width*height ? 3 : 0;
}

}
