import time
from .player import Player, TimeOutException, InvalidMoveException


class Game:
    '''A Game instance runs a game between two ias. It manages the game's progress
        and checks players actions.

    inputs:
        - ia1: the starting artificial intel
        - ia2: the other artificial intelligence

    outputs:
        - pgn: the Portable Game Notation summary of the game
    '''

    def __init__(self, module, ia1, timeLimit1, ia2, timeLimit2, gameConfig={}):
        self.gameState = module.GameState(**gameConfig)
        self.player1 = Player(True, ia1, timeLimit1)
        self.player2 = Player(False, ia2, timeLimit2)

        self.displayLevel = 0
        self.pause = 0
        self.log = ""
        self.status = {'success': False, 'draw': None,
                       'winner': None, 'playerError': None, 'errorID': None}

    def init_logs(self):
        self.addToLog("Beginning of the game")
        self.addToLog(str(self.player1) + ' (starts) has ' +
                      str(self.player1.timeLimit) + ' secs/turn to play')
        self.addToLog(str(self.player2) + ' has ' +
                      str(self.player1.timeLimit) + ' secs/turn to play')
        self.logState()

    def start(self):
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
            termination = self.gameState.checkTermination()
            if termination == 3:
                result = '1/2-1/2'
                self.status['success'] = True
                self.status['draw'] = True
                self.addToLog('Draw', 1)
                break
            elif termination > 0:
                assert termination in (1, 2)
                result = '1-0' if termination == 1 else '0-1'
                self.status['success'] = True
                self.status['winner'] = termination
                self.addToLog('End of Game !', 1)
                break
            possibleMoves = self.gameState.findPossibleMoves()
            self.logChoices(possibleMoves)

            time.sleep(max(0, t - time.time()))
            t = time.time() + self.pause

            # make the player play
            try:
                chosenState = player.play(self.gameState.copy())
            except TimeOutException as exc:
                self.status['playerError'] = playerNumber
                self.status['errorID'] = 'T0'
                self.addToLog(exc.message, 0)
                return
            except InvalidMoveException as e:
                self.status['playerError'] = playerNumber
                self.status['errorID'] = 'IM'
                self.addToLog('Invalid move from ' + str(player) + e.message, 0)
                return
            except Exception as e:
                self.status['playerError'] = playerNumber
                self.status['errorID'] = 'Unknown'
                self.addToLog(str(e), 0)
                return

            # find and add the chosen move to pgn
            chosenStateRepr = repr(chosenState)
            possibleRepr = [repr(self.gameState.copy().doMove(move)) for move in possibleMoves]
            if chosenStateRepr not in possibleRepr:
                print('Player returned an invalid state.')
                self.gameState.display()
                chosenState.display()
                raise Exception(chosenStateRepr + '\n not in \n' + '\n'.join(possibleRepr))
            move = possibleMoves[possibleRepr.index(chosenStateRepr)]
            if player is self.player1:
                pgnMoves += str(turn // 2 + 1) + "."
            pgnMoves += str(move) + " "

            self.gameState = chosenState

            # log the computing time and game state
            self.logCompuTime(player.computingTimes[-1])
            self.logState()

        # create the PGN of the game
        self.pgn = self.makePGN(startTime, pgnMoves, result)
        self.log += "\n#PGN#\n" + self.pgn

# End of start

    stateDisplayLevel = 1
    choiceDisplayLevel = 2
    decisionDisplayLevel = 2

    def addToLog(self, txt, displayLevel=1):
        self.log += txt + '\n'
        if self.displayLevel >= displayLevel:
            print(txt)

    def logState(self):
        self.addToLog(self.gameState.toDisplay(), Game.stateDisplayLevel)

    def logChoices(self, possibleMoves):
        recap = "Possible moves :\n"
        for m in possibleMoves:
            recap += str(m) + "\n"
        self.addToLog(recap, Game.choiceDisplayLevel)

    def logCompuTime(self, t):
        self.addToLog("Computing time : " + str(t) + " sec(s)", Game.choiceDisplayLevel)

    def makePGN(self, startTime, pgnMoves, result):
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
