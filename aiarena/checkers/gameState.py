from .boardState import BoardState

defaultConfig = {'nRows': 8,
                 'nPieces': 12,
                 'whiteStarts': True}

CheckersRules = {'menCaptureBackward': False,
                 'kingsCanFly': False,
                 'menMustStop': True,
                 'noCaptureMax': 16}

InternatRules = {'menCaptureBackward': True,
                 'kingsCanFly': True,
                 'menMustStop': False,
                 'noCaptureMax': 25}

defaultRules = CheckersRules


class GameState:
    ''' The GameState gathers the state of the board plus some auxilliary info
    like whose turn to play and info to know if it is a draw '''

    def __init__(self, config=defaultConfig, rules=defaultRules):
        self.boardState = BoardState(config, rules)
        self.isWhiteTurn = config['whiteStarts']
        self.noCaptureCounter = 0

    def copy(self):
        copy = GameState()
        copy.boardState = self.boardState.copy()
        copy.noCaptureCounter = self.noCaptureCounter
        copy.isWhiteTurn = self.isWhiteTurn
        return copy

    def findPossibleMoves(self):
        return self.boardState.findPossibleMoves(self.isWhiteTurn)

    def doMove(self, move):
        self.boardState.doMove(move)
        self.noCaptureCounter = 0 if move.isCapture() else self.noCaptureCounter + 1
        self.isWhiteTurn = not self.isWhiteTurn
        return self

    def findNextStates(self):
        moves = self.findPossibleMoves()
        nextStates = []
        for move in moves:
            nextStates.append(self.copy().doMove(move))
        return nextStates

    def reverse(self):
        self.boardState.reverse()
        self.isWhiteTurn = not self.isWhiteTurn
        return self

    def checkTermination(self):
        if len(self.findPossibleMoves()) == 0:
            current_player_index = 1 if (
                self.boardState.config['whiteStarts'] == self.isWhiteTurn) else 2
            return 1 if current_player_index == 2 else 2
        if self.noCaptureCounter >= 25:
            return 3
        return 0

    def __repr__(self):
        return self.toString(True, True)

    def toString(self, turn=True, counts=False):
        s = repr(self.boardState)
        if turn:
            s += " W" if self.isWhiteTurn else " B"
        if counts:
            s += f" {self.noCaptureCounter}"
        return s

    def toDisplay(self, showBoard=False):
        s = self.boardState.toDisplay(showBoard) + '\n'
        if self.isWhiteTurn:
            s += "White's turn to play."
        else:
            s += "Black's turn to play."
        return s

    def display(self, showBoard=False):
        print(self.toDisplay(showBoard))
