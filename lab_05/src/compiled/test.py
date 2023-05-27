import threading

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

        if (op=='*'):
            threads.append(threading.Thread(target = print((a * b))))
            threads[-1].start()

        if (op=='/'):
            threads.append(threading.Thread(target = print((a / b))))
            threads[-1].start()

        if (op=='-'):
            threads.append(threading.Thread(target = print((a - b))))
            threads[-1].start()

        stopCommand=input()

    for thread in threads:
        thread.join()


