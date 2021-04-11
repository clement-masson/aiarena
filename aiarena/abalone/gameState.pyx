# cython: language_level=3
from cython.operator cimport dereference as deref
from .gameState cimport *
from colorama import Fore, Back, Style
from . import cell

ASCI_TXT = False


cdef class GameState:
    '''
    This class is a Pyhton interface (front-end) for the C++ backend of CGameState.
    It also contains some non-critic functions (whose performance is not critical) like displaying the board
    '''

    def __cinit__(self, int size=DEFAULT_SIZE):
          self.cGameState = new CGameState(size)

    def __dealloc__(self):
        del self.cGameState

    def copy(self):
        copy = GameState()
        del copy.cGameState
        copy.cGameState = new CGameState( deref(self.cGameState) )
        return copy

    @property
    def size(self):
      return self.cGameState.size

    @property
    def isWhiteTurn(self):
      return self.cGameState.isWhiteTurn

    @property
    def capturedWhiteBalls(self):
      return self.cGameState.capturedWhiteBalls

    @property
    def capturedBlackBalls(self):
      return self.cGameState.capturedBlackBalls

    @property
    def cells(self):
      return [Cell.wrap(e) for e in self.cGameState.cells]

    def getCell(self, int i, int j):
      return Cell.wrap(self.cGameState.getCell(i, j))

    def getRow(self, int i):
      cdef vector[CCell] temp_var = self.cGameState.getRow(i)
      return [Cell.wrap(e) for e in temp_var]

    def getColumn(self, int i):
      cdef vector[CCell] temp_var = self.cGameState.getColumn(i)
      return [Cell.wrap(e) for e in temp_var]

    def getDiagonal(self, int i):
      cdef vector[CCell] temp_var = self.cGameState.getDiagonal(i)
      return [Cell.wrap(e) for e in temp_var]

    def reverse(self):
        self.cGameState.reverse()
        return self

    def __repr__(self):
        return self.cGameState.toString().decode('UTF-8')

    def toString(self):
        return self.cGameState.toString().decode('UTF-8')

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
        cdef CMove* c = move.cMove
        self.cGameState.doMove( deref(c) )
        return self

    def findNextStates(self):
        moves = self.findPossibleMoves()
        nextStates = []
        for m in moves:
            nextStates.append( self.copy().doMove(m) )
        return nextStates

    def checkTermination(self):
        return self.cGameState.checkTermination()


    '''
    Visualization methods
    '''
    def toDisplay(self, showBoard=False):
        ''' Return a string suitable for state visualization in text mode (like the one at the top of this file)
        If showBard is True, then a board with cell indices is shown next to the state'''
        max_width = self.cGameState.getWidth(self.size-1)
        piece_asci_len = 1 if ASCI_TXT else 1
        number_len = 2
        formater = '{0:'+str(number_len)+'d}'

        s = " ,"+('-'*piece_asci_len*(2*max_width))+","
        if showBoard:
            s+= "    ,"+('-'*piece_asci_len*(2*max_width))+","
        s +="\n"
        for r in reversed(range(2*self.size-1)):
            padding = max_width - self.cGameState.getWidth(r)
            line = ' |' + ' '*padding
            for cell in self.getRow(r):
                line +=  get_ascii(cell) + ' '
            s += line
            s += Style.RESET_ALL + ' '*padding + '|'
            if showBoard:
                s+='    |'
                s += ' '*padding
                for c, cell in enumerate(self.getRow(r)):
                    col = c + self.cGameState.getStartOffset(r)
                    back = Back.BLUE if col%2 != r%2 else Back.LIGHTBLUE_EX
                    s += back + formater.format(self.RCtoIndex(r,col))
                s += Style.RESET_ALL + ' '*padding + '|'
            s += '\n'
        s += Style.RESET_ALL + " '"+('-'*piece_asci_len*(2*max_width))+"'"
        if showBoard:
            s += "    '"+('-'*piece_asci_len*(2*max_width))+"'"
        # s += "\n  " + ''.join([('{:'+str(piece_asci_len)+'s}').format(str(k)) for k in range(self.width)]) + " "
        s += '\n'
        if self.isWhiteTurn:
            s += "White's turn to play."
        else:
            s += "Black's turn to play."
        s += f" ({self.capturedWhiteBalls}:{self.capturedBlackBalls})"
        return s

    def display(self, showBoard=False):
        print(self.toDisplay(showBoard))


asci_symbols = {
  cell.NONE: '\u25CF',
  cell.WHITE: '\u25CF',
  cell.BLACK: '\u25CF',
}
#
#
def get_ascii(piece):
  if piece.color is cell.WHITE:
    fore = Fore.WHITE
  elif piece.color is cell.BLACK:
    fore = Fore.RED
  else:
    fore = Fore.BLACK
  if ASCI_TXT:
    return Style.BRIGHT + fore + 'o'
  else:
    return Style.BRIGHT + fore + asci_symbols[piece.color]
