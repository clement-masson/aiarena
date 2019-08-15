import sys
import subprocess as sp
from .misc import smart_display


class ManualBrain:

    def __init__(self):
        print("Please enter your name")
        self.name = sys.stdin.readline()[0:-1]
        self.alwaysSeeAsWhite = False

    def play(self, gameState, timeLimit):
        possibleMoves = gameState.findPossibleMoves()
        sp.check_call('clear')
        print(gameState.toDisplay(True))
        print("Authorized moves : ")
        pdn_list = [m.toPDN() for m in possibleMoves]
        smart_display(pdn_list, count=True)

        while True:
            print("Please enter a move")
            string = sys.stdin.readline()[0:-1]
            if string.isnumeric() and int(string) in range(len(possibleMoves)):
                return possibleMoves[int(string)].toPDN()
            elif string in pdn_list:
                return string
            print(string + ' is an invalid move !')

    def __str__(self):
        return self.name
