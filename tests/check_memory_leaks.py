import aiarena
import argparse
from get_function_memory_usage import get_function_memory_usage

ITERATIONS = 1000
VERBOSE = False


def check_method(method):
    print(f'  checking method {method.__name__}')
    memory_usage, _ = get_function_memory_usage(method, iterations=ITERATIONS,
                                                verbose=VERBOSE)
    assert memory_usage == 0
    print('  OK !')


def check_game(gametype):
    print(f'\nchecking game {gametype.__name__}')
    gameState = gametype.GameState()
    check_method(gameState.copy)
    check_method(gameState.findPossibleMoves)
    check_method(gameState.findNextStates)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--ITERATIONS', type=int, default=100)
    parser.add_argument('-v', '--VERBOSE', action='store_true', default=False)

    # la ligne ci-dessous remplace les valeurs des variables globales du module par les CL options   
    globals().update(vars(parser.parse_args()))

    check_game(aiarena.chess)
    check_game(aiarena.checkers)
    check_game(aiarena.connect4)
    check_game(aiarena.abalone)
