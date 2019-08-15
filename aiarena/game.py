import time
from .player import Player, TimeOutException


class Game:
    '''A Game instance runs a game between two ias. It manages the game's progress
        and checks players actions.

    inputs:
        - ia1: the starting artificial intel
        - ia2: the other artificial intelligence

    outputs:
        - pgn: the Portable Game Notation summary of the game
    '''

    def __init__(self, module, ia1, timeLimit1, ia2, timeLimit2):
        self.gameState = module.GameState()
        self.noCaptureMax = 50
        self.player1 = Player(True, ia1, timeLimit1)
        self.player2 = Player(False, ia2, timeLimit2)

        self.displayLevel = 0
        self.pause = 0
        self.log = ""
        self.status = {'success': False, 'draw': False,
                       'winner': None, 'playerError': None, 'errorID': None}

    def init_logs(self):
        self.addToLog("Beginning of the game")
        self.addToLog(str(self.player1) + ' (starts) has ' +
                      str(self.player1.timeLimit) + ' secs/turn to play')
        self.addToLog(str(self.player2) + ' has ' +
                      str(self.player1.timeLimit) + ' secs/turn to play')
        self.logState()

    def runGame(self):
        # setup
        self.init_logs()
        self.pgn = None
        pgnMoves = ""
        result = None
        startTime = time.ctime()
        time.sleep(self.pause)
        t = time.time() + self.pause

        # shortcut variables to acces faster white and black players
        whitePlayer = self.player1 if self.player1.isWhite else self.player2
        blackPlayer = self.player2 if self.player1.isWhite else self.player1
        turn = 0

        while True:
            turn += 1
            player = whitePlayer if self.gameState.isWhiteTurn else blackPlayer
            playerNumber = 1 if player is self.player1 else 2

            # Check whether the game is ended
            possibleMoves = self.gameState.findPossibleMoves()
            if not possibleMoves:
                result = '0-1' if player is self.player1 else '1-0'
                self.status['success'] = True
                self.status['winner'] = 3 - playerNumber
                self.addToLog('End of Game !', 1)
                break
            self.logChoices(possibleMoves)

            time.sleep(max(0, t - time.time()))
            t = time.time() + self.pause

            # make the player play
            try:
                chosenMove = player.play(self.gameState.copy())
            except TimeOutException as exc:
                self.status['playerError'] = playerNumber
                self.status['errorID'] = 'T0'
                self.addToLog(exc.message, 0)
                return
            except Exception as e:
                self.status['playerError'] = playerNumber
                self.status['errorID'] = 'Unknown'
                try:
                    self.addToLog(e.message, 0)
                except Exception:
                    pass
                raise e

            # check whether the answer is valid
            move = None
            for m in possibleMoves:
                if chosenMove.toPDN() == m.toPDN():
                    move = m
                    break
            if not move:
                self.status['playerError'] = playerNumber
                self.status['errorID'] = 'IM'
                self.addToLog('Invalid move from ' + str(player), 0)
                return
            self.gameState.doMove(move, inplace=True)

            # log the game
            self.logCompuTime(player.computingTimes[-1])
            self.logState()
            if player is self.player1:
                pgnMoves += str(turn // 2 + 1) + "."
            pgnMoves += move.toPDN() + " "

            # Check if it is a Draw
            if self.gameState.noCaptureCounter >= self.noCaptureMax:
                result = '1/2-1/2'
                self.status['success'] = True
                self.status['draw'] = True
                self.addToLog('Draw', 1)
                break

        # If the simulation stops before the game ends
        if not self.status['success']:
            self.status['errorID'] = 'MAX'
            self.addToLog('The game could not end within ' + str(self.Nlimit) + ' steps')
            return

        # create the PDN of the game
        self.pgn = self.makePDN(startTime, pgnMoves, result)
        self.log += "\n#PDN#\n" + self.pgn

# End of runGame

    stateDisplayLevel = 1
    choiceDisplayLevel = 2
    decisionDisplayLevel = 2

    def addToLog(self, txt, displayLevel=1):
        self.log += txt + '\n'
        if self.displayLevel >= displayLevel:
            print(txt)

    def logState(self):
        self.addToLog(self.gameState.toDisplay(True), Game.stateDisplayLevel)

    def logChoices(self, possibleMoves):
        recap = "Possible moves :\n"
        for m in possibleMoves:
            recap += m.toPDN() + "\n"
        self.addToLog(recap, Game.choiceDisplayLevel)

    def logCompuTime(self, t):
        self.addToLog("Computing time : " + str(t) + " sec(s)", Game.choiceDisplayLevel)

    def makePDN(self, startTime, pgnMoves, result):
        # shortcut variables to acces faster white and black players
        whitePlayer = self.player1 if self.player1.isWhite else self.player2
        blackPlayer = self.player2 if self.player1.isWhite else self.player1

        pgn = '[Event "IN104 CS Project"]\n'
        pgn += '[Site "ENSTA ParisTech, Palaiseau, FRA"]\n'
        pgn += '[Date "' + startTime + '"]\n'
        pgn += '[Round "?"]\n'
        pgn += '[White "' + whitePlayer.name() + '"]\n[WhiteType "Program"]\n'
        pgn += '[Black "' + blackPlayer.name() + '"]\n[BlackType "Program"]\n'

        pgn += '[Result "' + result + '"]'
        if result == '1-0':
            pgn += ' {' + str(self.player1) + ' wins}'
        elif result == '0-1':
            pgn += ' {' + str(self.player2) + ' wins}'

        pgn += '\n[WhiteTime "' + str(sum(whitePlayer.computingTimes)) + '"]\n'
        pgn += '[BlackTime "' + str(sum(blackPlayer.computingTimes)) + '"]\n\n'
        pgn += pgnMoves + ' ' + result + ' *' + '\n'
        return pgn
