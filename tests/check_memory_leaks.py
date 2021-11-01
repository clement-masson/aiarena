import os
import time
import psutil
import argparse

import aiarena

ITERATIONS = 1000
VERBOSE = False


def get_function_memory_usage(function, iterations=100, max_time=None,
                                max_mem_usage=90, verbose=False, display_period=0.2):
    '''
    Cette fonction execute <iterations> fois <function> et renvoit la difference
    de mémoire virtuelle utilisée par le processus python entre la fin des
    iterations et le début. Une mesure de sécurité s'assure qu'une proportion
    <max_mem_usage> (en %, entre 0 et 100, defaut 90) de la RAM totale n'est pas dépassée
    au cours du bench afin de ne pas bloquer l'ordinateur.
    '''
    this_process = psutil.Process(os.getpid())
    i = 0
    t0 = time.time()
    last_display_time = t0
    last_display_iteration = i
    initial_process_mem = this_process.memory_info().vms

    while True:
        # execution de la fonction a bencher
        function()
        i += 1

        # recuperation l'heure et de la mémoire utilisée
        process_mem = this_process.memory_info().vms
        total_mem = psutil.virtual_memory()
        t2 = time.time()

        # Affichage
        if verbose and display_period > 0 and (t2 - last_display_time) > display_period:
            function_mem = process_mem - initial_process_mem
            total_test_time = t2 - t0
            average_iteration_duration = (
                t2 - last_display_time) / (i - last_display_iteration)

            time_display = f'{total_test_time:.1f}s [{average_iteration_duration*1000:.1f}ms/it]'
            function_mem_display = f'function: {function_mem/2**20:.0f}MB'
            total_mem_display=f'total: {total_mem.used/2**20:.0f}MB ({total_mem.percent:.0f}%)'
            print(i, time_display, function_mem_display,
                  total_mem_display, end = '\r')
            last_display_time=t2
            last_display_iteration=i

        if i >= iterations:
            if verbose:
                print(f'Reached maximum iteration number {iterations}')
            break

        # Check les critere d'arret
        if total_mem.percent >= max_mem_usage:
            if verbose:
                print(f'Reached maximum memory usage ({max_mem_usage}%) after {i} iterations')
            break

        if max_time is not None and t2 - t0 >= max_time:
            if verbose:
                print(f'Reached maximum duration {max_time} after {i} iterations')
            break

    # Preparation de la sortie
    return (process_mem - initial_process_mem), i


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
