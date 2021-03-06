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
        gameState.display(showBoard=True)
        print("Authorized moves : ")
        pdn_list = [str(m) for m in possibleMoves]
        smart_display(pdn_list, count=True)

        while True:
            print("Please enter a move")
            string = sys.stdin.readline()[0:-1]
            if string in pdn_list:
                return possibleMoves[pdn_list.index(string)]
            elif string.isnumeric() and int(string) in range(len(possibleMoves)):
                return possibleMoves[int(string)]
            print(string + ' is an invalid move !')

    def __str__(self):
        return self.name
