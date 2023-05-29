import threading
import datetime

########################################

threads = []

if __name__ == '__main__':
    stopCommand=input()
    while not(stopCommand=='q'):
        print('enter a command:')
        a=int(input())
        op=input()
        b=int(input())
        if (op=='+'):
            threads.append(threading.Thread(target = print((a + b))))
            threads[-1].start()
            print(f'thread {len(threads)} started at ', datetime.datetime.now().time())

        if (op=='*'):
            threads.append(threading.Thread(target = print((a * b))))
            threads[-1].start()
            print(f'thread {len(threads)} started at ', datetime.datetime.now().time())

        if (op=='/'):
            threads.append(threading.Thread(target = print((a / b))))
            threads[-1].start()
            print(f'thread {len(threads)} started at ', datetime.datetime.now().time())

        if (op=='-'):
            threads.append(threading.Thread(target = print((a - b))))
            threads[-1].start()
            print(f'thread {len(threads)} started at ', datetime.datetime.now().time())

        stopCommand=input()

    for i in range(len(threads)):
        threads[i].join()
        print(f'thread {i + 1} finished at', datetime.datetime.now().time())


