import numpy as np
import timeit

def time_reg(x):
    try:
        N = int(input('Type in the number (as number) of loops: '))
    except ValueError:
        print('This is cannot be interpreted as integer.')
        return None
    time = timeit.timeit(stmt = 'x ** 2 - x ** 2 + x ** 4 - x ** 5 + x + x', globals = {'x': x}, number = N)
    return N, time

def main():
    data_list = []
    ans = 'y'
    while ans == 'y':
        x = np.random.random()
        data = time_reg(x)
        if data == None:
            break
        print('For %i repeats the elapsed time is %.2e sec'%data)
        data_list.append(data)
        ans = input('Continue? [y/n]: ')
    print(data_list)
    print('Goodbye.')
    return 0

main()