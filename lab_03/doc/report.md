# ЛР 3: C++ CLI / FUNCTION / LOOP / RECURSION
## Цель:

Познакомить студента с основными алгоритмическими конструкциями, которые будут использоваться для создания CLI программы. Далее продемонстрировать эффективность использования механизма рекурсии.

С++ алгоритмы: CLI Калькулятор вещественных чисел +, -, ^, . Реализация с
использованием только функций, условий, циклов, + и -. Вид команд в консоли: calc
plus / minus / power; Ханойская башня, результат корректной последовательности

## Задача 1: **[С++ CLI CALC] Создать программу CALC с интерфейсом CLI**

Создать программу под названием CALC, которая будет принимать на вход 3
аргумента (2 операнда и 1 оператор). Оператор может быть: +, -, ^. Реализация
операторов только с использованием функций, условий, циклов, +, - и *.

### Решение:

В параметры ```main``` подаётся три значения: два операнда ```x1``` и ```x2``` и оператор ```op```. Программа вызывается через PowerShell с указанием параметров в следующем виде:

```.\lab_03_calc.exe x1 op x2```

На выходе программа выдаёт результат в соответствии с параметрами.

## Задача 2: **[C++ RECURSION] рекурсии**

Описание: Ханойская башня является одной из популярных головоломок XIX века. Даны три стержня, на один из которых нанизаны восемь колец, причём кольца отличаются размером и лежат меньшее на большем. Задача состоит в том, чтобы перенести пирамиду из восьми колец за наименьшее число ходов на другой стержень. За один раз разрешается переносить только одно кольцо, причём нельзя класть большее кольцо на меньшее.

Результат обнаруженной последовательности шагов записать в виде двусвязного
списка. В конце программы сделать вывод этого списка на экран. Освободить
память списка перед завершением программы.

### Решение:

Задача решается рекурсивным образом. Глобально задача разбивается на следующие шаги:

1. Рекурсивно переносим башню из $n-1$ диска со штыря A на штырь В.
2. Переносим самый большой диск со штыря A на штырь С
3. Рекурсивно переносим башню $n - 1$ диска со штыря В на штырь С. 

Блок-схема:

![image](https://user-images.githubusercontent.com/82118458/233734186-5ed8b319-b4f9-46bc-a8b1-214b665097bf.png)

После переноса башни ```show_list``` выводит список действий, совершаемых для решения головоломки.

## Вывод:

Я научился работать с параметрами функции и составлением рекурсивных функций.
