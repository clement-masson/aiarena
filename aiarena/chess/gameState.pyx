# cython: language_level=3
from cython.operator cimport dereference as deref
from .gameState cimport *
from colorama import Fore, Back, Style

ASCI_TXT = False


cdef class GameState:
    '''
    This class is a Pyhton interface (front-end) for the C++ backend of CGameState.
    It also contains some non-critic functions (whose performance is not critical) like displaying the board
    '''

    nrows = NROWS
    ncolulns = NCOLUMNS

    def __cinit__(self):
          self.cGameState = new CGameState()

    def __dealloc__(self):
        del self.cGameState

    def copy(self):
        copy = GameState()
        del copy.cGameState
        copy.cGameState = new CGameState( deref(self.cGameState) )
        return copy

    @property
    def isWhiteTurn(self):
      return self.cGameState.isWhiteTurn

    @property
    def noPawnNoCapture(self):
      return self.cGameState.noPawnNoCapture

    @property
    def cells(self):
      return [Cell.wrap(e) for e in self.cGameState.cells]

    def getCell(self, int i, int j):
      return Cell.wrap(self.cGameState.getCell(i, j))

    def reverse(self):
        self.cGameState.reverse()
        return self

    def __repr__(self):
        return self.cGameState.getFEN(True, True, True).decode('UTF-8')

    def toString(self, turn=True, castle=True, counts=False):
        return self.cGameState.getFEN(turn, castle, counts).decode('UTF-8')

    def RCtoIndex(self, int r, int c):
        return self.cGameState.RCtoIndex(r,c)

    def indexToRC(self, int i):
        return self.cGameState.indexToRC(i)

    '''
    Core methods
    '''

    def findPossibleMoves(self):
        cdef vector[CMove*] cmoves = self.cGameState.findPossibleMoves()
        return [Move.wrap(m) for m in cmoves]

    def doMove(self, Move move):
        self.cGameState.doMove( deref(move.cMove) )
        return self

    def findNextStates(self):
        moves = self.findPossibleMoves()
        nextStates = []
        for m in moves:
            nextStates.append( self.copy().doMove(m) )
        return nextStates

    def checkTermination(self):
        if len(self.findPossibleMoves()) == 0:
            if self.cGameState.isInCheck():
                return 2 if self.isWhiteTurn else 1  # MAT
            else:
                return 3  # PAT
        if self.noPawnNoCapture >= 100:
            return 3  # DRAW
        return 0  # NOT TERMINATED


    '''
    Visualization methods
    '''
    def toDisplay(self, showBoard = False):
        ''' Return a string suitable for state visualization in text mode (like the one at the top of this file)
        If showBard is True, then a board with cell indices is shown next to the state'''
        cdef CCell cell

        piece_asci_len = 3 if ASCI_TXT else 2
        number_len = 3
        formater = '{0:'+str(number_len)+'d}'

        s = " ,"+('-'*piece_asci_len*NCOLUMNS)+","
        if showBoard:
            s+= "    ,"+('-'*number_len*NCOLUMNS)+","
        s +="\n"
        for r in reversed(range(NROWS)):
            s += str(r+1) + '|'
            for c in range(NCOLUMNS):
                cell = self.cGameState.getCell(r,c)
                back = Back.BLUE if c%2 == r%2 else Back.LIGHTBLUE_EX
                s +=  back + get_ascii(chr(cell.pieceType), cell.isWhite)
            s += Style.RESET_ALL + '|'
            if showBoard:
                s+='    |'
                for c in range(NCOLUMNS):
                    back = Back.BLUE if c%2 == r%2 else Back.LIGHTBLUE_EX
                    s += back + formater.format(self.RCtoIndex(r,c))
                s += Style.RESET_ALL + '|'
            s += '\n'
        s += Style.RESET_ALL + " '"+('-'*piece_asci_len*NCOLUMNS)+"'"
        if showBoard:
            s += "    '"+('-'*number_len*NCOLUMNS)+"'"
        s += "\n  " + ''.join([('{:'+str(piece_asci_len)+'s}').format(e) for e in 'ABCDEFGH']) + " "

        if self.isWhiteTurn:
            s += "White's turn to play."
        else:
            s += "Black's turn to play."
        return s

    def display(self, showBoard = False):
        print(self.toDisplay(showBoard))


asci_symbols = {
  ('R', True): '\u2656 ',
  ('N', True): '\u2658 ',
  ('B', True): '\u2657 ',
  ('Q', True): '\u2655 ',
  ('K', True): '\u2654 ',
  ('P', True): '\u2659 ',
  ('R', False): '\u265C ',
  ('N', False): '\u265E ',
  ('B', False): '\u265D ',
  ('Q', False): '\u265B ',
  ('K', False): '\u265A ',
  ('P', False): '\u265F ',
  (' ', True): '  ',
  (' ', False): '  '
}


def get_ascii(piece, isWhite):
  if ASCI_TXT:
    fore = Fore.WHITE if isWhite else Fore.RED
    return Style.BRIGHT + fore + ' ' + piece + ' '
  else:
    fore = Fore.WHITE if isWhite else Fore.RED
    return Style.BRIGHT + fore + asci_symbols[(piece, False)]
