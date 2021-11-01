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

    def __cinit__(self, width=DEFAULT_WIDTH, height=DEFAULT_HEIGHT):
          self.cGameState = new CGameState(width, height)

    def __dealloc__(self):
        del self.cGameState

    def copy(self):
        copy = GameState()
        del copy.cGameState
        copy.cGameState = new CGameState( deref(self.cGameState) )
        return copy

    @property
    def width(self):
      return self.cGameState.width

    @property
    def height(self):
      return self.cGameState.height

    @property
    def isWhiteTurn(self):
      return self.cGameState.isWhiteTurn

    def setIsWhiteTurn(self, bool b):
        self.cGameState.isWhiteTurn = b

    @property
    def cells(self):
      return self.getCells()

    def getCell(self, int r, int c):
        return Cell.wrap(self.cGameState.getCell(r, c))

    def getCellById(self, int id):
        return Cell.wrap(self.cGameState.getCell(id))

    def getCells(self):
        return [Cell.wrap(e) for e in self.cGameState.cells]

    def setCell(self, int r, int c, Cell cell):
        return self.cGameState.setCell(r, c, cell.cCell)

    def setCellById(self, int id, Cell cell):
        return self.cGameState.setCell(id, cell.cCell)

    def setCells(self, cells):
        assert len(cells) == len(self.cells)
        for id, cell in enumerate(cells):
            self.setCellById(id, cell)

    def setCellsFromString(self, repr):
        return self.cGameState.setCellsFromString(repr.encode('UTF-8'))

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
        piece_asci_len = 3 if ASCI_TXT else 2
        s = " ,"+('-'*piece_asci_len*self.width)+","
        s +="\n"
        for r in reversed(range(self.height)):
            s += ' |'
            for c in range(self.width):
                cell = self.getCell(r,c)
                back = Back.BLUE if c%2 == r%2 else Back.LIGHTBLUE_EX
                s +=  back + get_ascii(cell)
            s += Style.RESET_ALL + '|'
            s += '\n'
        s += Style.RESET_ALL + " '"+('-'*piece_asci_len*self.width)+"'"
        if showBoard:
            s += "\n  " + ''.join([('{:'+str(piece_asci_len)+'s}').format(str(k)) for k in range(self.width)]) + " "

        if self.isWhiteTurn:
            s += "\nWhite's turn to play."
        else:
            s += "\nBlack's turn to play."
        return s

    def display(self, showBoard=False):
        print(self.toDisplay(showBoard))


asci_symbols = {
  cell.NONE: '  ',
  cell.WHITE: '\u25CF ',
  cell.BLACK: '\u25CF ',
}


def get_ascii(piece):
  if ASCI_TXT:
    fore = Fore.WHITE if piece.color is cell.WHITE else Fore.RED
    return Style.BRIGHT + fore + ' ' + piece + ' '
  else:
    fore = Fore.WHITE if piece.color is cell.WHITE else Fore.RED
    return Style.BRIGHT + fore + asci_symbols[piece.color]
