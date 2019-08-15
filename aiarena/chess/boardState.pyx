# cython: language_level=3
from cython.operator cimport dereference as deref, preincrement as inc
from .boardState cimport CBoardState, NROWS, NCOLUMNS
from .cell import PyCell
from colorama import Fore, Back, Style

ASCI_TXT = False


cdef class BoardState:
    '''
    This class is a Pyhton interface (front-end) for the C++ backend of CBoardState.
    It also contains some non-critic functions (whose performance is not critical) like displaying the board
    '''

    nrows = NROWS
    ncolulns = NCOLUMNS

    def __cinit__(self):
          self.cBoardState = new CBoardState()

    def __dealloc__(self):
        del self.cBoardState

    def copy(self):
        copy = BoardState()
        copy.cBoardState = new CBoardState( deref(self.cBoardState) )
        return copy

    @property
    def isWhiteTurn(self):
      return self.cBoardState.isWhiteTurn

    # @property
    # def cells(self):
    #   return [PyCell.wrap(e) for e in self.cBoardState.cells]

    # cdef getCell(BoardState self, int i, int j):
      # return

    # def getCell2(self, i, j):
    #     return self.getCell(i, j)

    def reverse(self):
        self.cBoardState.reverse()
        return self

    def __repr__(self):
        return self.cBoardState.getFEN(True, True, True).decode('UTF-8')

    def getFEN(self, turn=True, castle=True, counts=True):
        return self.cBoardState.getFEN(turn, castle, counts).decode('UTF-8')

    def RCtoIndex(self, int r, int c):
        return self.cBoardState.RCtoIndex(r,c)

    def indexToRC(self, int i):
        return self.cBoardState.indexToRC(i)

    '''
    Core methods
    '''

    def findPossibleMoves(self):
        cdef vector[CMove*] cmoves = self.cBoardState.findPossibleMoves()
        return [Move.wrap(m) for m in cmoves]

    def doMove(self, Move move):
        cdef CMove* c = move.cMove
        self.cBoardState.doMove( deref(c) )
        return self

    def findNextStates(self):
        moves = self.findPossibleMoves()
        nextStates = []
        for m in moves:
            nextStates.append( self.copy().doMove(m) )
        return nextStates


    '''
    Visualization methods
    '''
    def toDisplay(self, showBoard = False):
        ''' Return a string suitable for state visualization in text mode (like the one at the top of this file)
        If showBard is True, then a board with cell indices is shown next to the state'''

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
                cell = self.cBoardState.getCell(r,c)
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
  ('o', True): '\u2659 ',
  ('R', False): '\u265C ',
  ('N', False): '\u265E ',
  ('B', False): '\u265D ',
  ('Q', False): '\u265B ',
  ('K', False): '\u265A ',
  ('o', False): '\u265F ',
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
