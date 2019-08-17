from cython.operator cimport dereference as deref, preincrement as inc
from colorama import Fore, Back, Style
from .boardState cimport *

ASCI_TXT = False


cdef class BoardState:
    '''
    This class is a Pyhton interface (front-end) for the C++ backend of CBoardState.
    It also contains some non-critic functions (whose performance is not critical) like displaying the board
    '''

    property debug:
        def __get__(self): return self.debug
        def __set__(self, bool b): self.debug = b

    property nRows:
        def __get__(self): return self.cBoardState.nRows

    property nCells:
        def __get__(self): return self.cBoardState.nCells

    property nPieces:
        def __get__(self): return self.cBoardState.nPieces

    property cells:
        def __get__(self): return self.cBoardState.getCells()


    def __cinit__(self, config=None, rules=None):
        self.debug = False
        if config:
            if not rules: raise Exception('You must rovides configuration AND rules')
            self.cBoardState = new CBoardState( config['nRows'],
                                                config['nPieces'],
                                                rules['menCaptureBackward'],
                                                rules['kingsCanFly'],
                                                rules['menMustStop'])

    def __dealloc__(self):
        del self.cBoardState

    def copy(self):
        copy = BoardState()
        copy.cBoardState = new CBoardState( deref(self.cBoardState) )
        return copy

    def reverse(self):
        self.cBoardState.reverse()
        return self

    def __repr__(self):
        return self.cBoardState.toString().decode('UTF-8')

    '''
    Checkers, converters, getters and setter
    '''
    def isValidIndex(self, int i):
        return self.cBoardState.isValidIndex(i)

    def isValidRC(self, int r, int c):
        return self.cBoardState.isValidRC(r,c)

    def RCtoIndex(self, int r, int c):
        return self.cBoardState.RCtoIndex(r,c)

    def indexToRC(self, int i):
        return self.cBoardState.indexToRC(i)

    def getCell(self, int r,int c):
        return self.cBoardState.getCell(r, c)

    def setCell_i(self, int i, char cell):
        self.cBoardState.setCell(i, cell)

    def setCell_rc(self, int r, int c, char cell):
        self.cBoardState.setCell(r, c, cell)


    '''
    Core methods
    '''

    def tryMoveFrom(self, int cellIndex):
        cdef vector[CSimpleMove*] cmoves = self.cBoardState.tryMoveFrom(cellIndex)
        return [Move.wrap(m) for m in cmoves]

    def tryJumpFrom(self, int cellIndex):
        cdef vector[CCaptureMove*] cmoves = self.cBoardState.tryJumpFrom(cellIndex)
        return [Move.wrap(m) for m in cmoves]

    def findPossibleMoves(self, bool white):
        cdef vector[CMove*] cmoves = self.cBoardState.findPossibleMoves(white)
        return [Move.wrap(m) for m in cmoves]

    def doMove(self, Move move):
        cdef CMove* c = move.cMove
        self.cBoardState.doMove( deref(c) )
        return self


    def findNextStates(self, bool white):
        moves = self.findPossibleMoves(white)
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

        nRows = self.cBoardState.nRows
        piece_asci_len = 3 if ASCI_TXT else 2
        number_len = 2
        formater = '{0:'+str(number_len)+'d}'

        s = " ,"+('-'*piece_asci_len*nRows)+","
        if showBoard:
            s+= "    ,"+('-'*number_len*nRows)+","
        s +="\n"
        for r in range(nRows):
            s += ' |'
            for c in range(nRows):
                s += Back.BLUE if c%2 == r%2 else Back.LIGHTBLUE_EX
                if c%2 != r%2:
                    s +=  get_ascii(chr(self.getCell(r,c)))
                else:
                    s += ' ' * piece_asci_len
            s += Style.RESET_ALL + '|'
            if showBoard:
                s += '    |'
                for c in range(nRows):
                    s += Back.BLUE if c%2 == r%2 else Back.LIGHTBLUE_EX
                    if c%2 != r%2:
                        s+=formater.format(self.RCtoIndex(r,c))
                    else:
                        s += ' ' * number_len
                s += Style.RESET_ALL + '|'

            s+='\n'
        s += Style.RESET_ALL + " '"+('-'*piece_asci_len*nRows)+"'"
        if showBoard:
            s+= "    '"+('-'*number_len*nRows)+"'"
        return s

    def display(self, showBoard = False):
        print(self.toDisplay(showBoard))

asci_symbols = {
  'w': '\u26C0 ',
  'W': '\u26C1 ',
  'b': '\u26C0 ',
  'B': '\u26C1 ',
  '.': '  ',
}


def get_ascii(piece):
  if ASCI_TXT:
    fore = Fore.WHITE if piece.lower() == 'w' else Fore.RED
    return Style.BRIGHT + fore + ' ' + piece + ' '
  else:
    fore = Fore.WHITE if piece.lower() == 'w' else Fore.BLACK
    return Style.BRIGHT + fore + asci_symbols[piece]
