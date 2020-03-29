from cython.operator cimport dereference as deref, preincrement as inc
from colorama import Fore, Back, Style
from .gameState cimport *

ASCI_TXT = False

defaultConfig = {'nRows': 8,
                 'nPieces': 12}

CheckersRules = {'menCaptureBackward': False,
                 'kingsCanFly': False,
                 'menMustStop': True,
                 'noCaptureMax': 16}

InternatRules = {'menCaptureBackward': True,
                 'kingsCanFly': True,
                 'menMustStop': False,
                 'noCaptureMax': 25}

defaultRules = CheckersRules


cdef class GameState:
    '''
    This class is a Pyhton interface (front-end) for the C++ backend of CGameState.
    It also contains some non-critic functions (whose performance is not critical) like displaying the board
    '''


    def __cinit__(self, config=defaultConfig, rules=defaultRules):
        if config is not None:
            if rules is None: raise Exception('You must provide configuration AND rules')
            self.cGameState = new CGameState( config['nRows'],
                                                config['nPieces'],
                                                rules['menCaptureBackward'],
                                                rules['kingsCanFly'],
                                                rules['menMustStop'])

    def __dealloc__(self):
        del self.cGameState

    def copy(self):
        copy = GameState(None, None)
        copy.cGameState = new CGameState( deref(self.cGameState) )
        return copy

    property nRows:
        def __get__(self): return self.cGameState.nRows

    property nCells:
        def __get__(self): return self.cGameState.nCells

    property nPieces:
        def __get__(self): return self.cGameState.nPieces

    @property
    def isWhiteTurn(self):
      return self.cGameState.isWhiteTurn

    @property
    def noCaptureCounter(self):
      return self.cGameState.noCaptureCounter

    @property
    def cells(self):
      return [Cell.wrap(e) for e in self.cGameState.cells]

    def getCell(self, int r,int c):
        return Cell.wrap(self.cGameState.getCell(r, c))

    def reverse(self):
        self.cGameState.reverse()
        return self

    def __repr__(self):
        return self.cGameState.toString(True, True).decode('UTF-8')

    def toString(self, turn=True, counts=False):
        return self.cGameState.toString(turn, counts).decode('UTF-8')

    '''
    Checkers, converters, getters and setter
    '''
    def isValidIndex(self, int i):
        return self.cGameState.isValidIndex(i)

    def isValidRC(self, int r, int c):
        return self.cGameState.isValidRC(r,c)

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
        if len(self.findPossibleMoves()) == 0:
            return 2 if self.isWhiteTurn else 1
        if self.noCaptureCounter >= 25:
            return 3
        return 0  # NOT TERMINATED


    '''
    Visualization methods
    '''
    def toDisplay(self, showBoard=False):
        ''' Return a string suitable for state visualization in text mode (like the one at the top of this file)
        If showBard is True, then a board with cell indices is shown next to the state'''

        nRows = self.cGameState.nRows
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
                    cell = self.cGameState.getCell(r,c)
                    s +=  get_ascii(chr(cell.pieceType), cell.isWhite)
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

        if self.isWhiteTurn:
           s += "\nWhite's turn to play."
        else:
           s += "\nBlack's turn to play."
        return s

    def display(self, showBoard=False):
        print(self.toDisplay(showBoard))


asci_symbols = {
  ('M', True): '\u26C0 ',
  ('K', True): '\u26C1 ',
  ('M', False): '\u26C0 ',
  ('K', False): '\u26C1 ',
  (' ', True): '  ',
  (' ', False): '  '
}


def get_ascii(piece, isWhite):
  if ASCI_TXT:
    fore = Fore.WHITE if isWhite else Fore.RED
    return Style.BRIGHT + fore + ' ' + piece + ' '
  else:
    fore = Fore.WHITE if isWhite else Fore.BLACK
    return Style.BRIGHT + fore + asci_symbols[(piece, isWhite)]
