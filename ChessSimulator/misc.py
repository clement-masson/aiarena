import subprocess as sp
import math


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
