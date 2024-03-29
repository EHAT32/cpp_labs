# ЛР 5: C++ OOP / PARALLEL

## Цель
Познакомить студента с принципами объектно-ориентированного программирования
на примере создания сложной синтаксической структуры. Придумать синтаксис своего
персонального мини-языка параллельного программирования, а также реализовать
его разбор и вычисление.

## Задача: Создать параллельный язык программирования

### Решение

Был создан язык программирования **Simple Language Allowing Turning Threads** (SLATT). Он имеет C-подобный синтаксис, но без ";". Важно, что каждый код начинается с "{" и заканчивается "}".

Суть работы: при запуске скрипта ```n_ary_tree.cpp``` в параметрах указываются путь до .slatt файла и путь, где будет создан скомпилированный код. ```n_ary_tree.cpp``` переводит код с .slatt в .py, который уже можно запускать.

Команды/возможности:

1. ```run!``` выполняет арифметическую операцию в отдельном потоке и выводит её. Также выводит время запуска потока.
2. ```read! [variable] as [type]``` задаёт переменной значение данного типа через ```input()```
3. ```write!``` соотетствует ```print()```
4. ```wait!``` завершает все запущенные потоки и пишет время завершения каждого.
5. Имеется возможность создавать условия ```if``` и циклы ```while```

## Вывод

При выполнении этой ЛР я приобрёл опыт работы с ООП, также $\pm$ изучил библиотеку, которая позволила сделать парсер.