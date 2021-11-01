import sys
import math
import subprocess as sp


def smart_display(liste, column_width=None, count=False):
    if count:
        ndigits = math.ceil(math.log10(len(liste)))
        pattern = '({:'+str(ndigits)+'d}) {}'
        liste = [pattern.format(k, e) for k, e in enumerate(liste)]
    if column_width is None:
        column_width = int(max(map(len, liste))) + 4
    disp_width = int(sp.getoutput('stty size').split()[1])
    n_columns = disp_width // column_width
    n_rows = math.ceil(len(liste) / n_columns)
    for r in range(n_rows):
        subl = liste[r*n_columns:(r+1)*n_columns]
        pattern = ('{:'+str(column_width)+'s}') * len(subl)
        print(pattern.format(*subl))


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
