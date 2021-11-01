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

    def setIsWhiteTurn(self, bool b):
      self.cGameState.isWhiteTurn = b

    @property
    def noPawnNoCapture(self):
      return self.cGameState.noPawnNoCapture

    def setNoPawnNoCapture(self, unsigned int c):
      self.cGameState.noPawnNoCapture = c

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

    def toString(self):
        return self.cGameState.toString().decode('UTF-8')

    def __repr__(self):
        return self.toString()

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
        return self.cGameState.checkTermination()

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
                back = Back.BLUE if c%2 == r%2 else Back.LIGHTBLUE_EX
                s +=  back + get_ascii(self.getCell(r,c))
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
  (cell.ROOK, True): '\u2656 ',
  (cell.KNIGHT, True): '\u2658 ',
  (cell.BISHOP, True): '\u2657 ',
  (cell.QUEEN, True): '\u2655 ',
  (cell.KING, True): '\u2654 ',
  (cell.PAWN, True): '\u2659 ',
  (cell.ROOK, False): '\u265C ',
  (cell.KNIGHT, False): '\u265E ',
  (cell.BISHOP, False): '\u265D ',
  (cell.QUEEN, False): '\u265B ',
  (cell.KING, False): '\u265A ',
  (cell.PAWN, False): '\u265F ',
  (cell.NONE, True): '  ',
  (cell.NONE, False): '  '
}


def get_ascii(cell):
  if ASCI_TXT:
    fore = Fore.WHITE if cell.isWhite else Fore.RED
    return Style.BRIGHT + fore + ' ' + cell.type + ' '
  else:
    fore = Fore.WHITE if cell.isWhite else Fore.RED
    return Style.BRIGHT + fore + asci_symbols[(cell.type, False)]
