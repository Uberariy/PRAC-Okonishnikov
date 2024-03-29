### **Отчет по Лабараторной работе №1**

**«Измерение времени работы программы»**

_Оконишников Арий Ариевич_

321 группа

Москва 2021

**Постановка задачи**

1. Измерить время выполнения программы с помощью разных инструментов.
2. Провести несколько измерений, посчитать среднее.
3. Изменить текст программы и настройки сборки так, чтобы уменьшить время выполнения.

**Конфигурация вычислительной системы**

| Процессор | Intel(R) Core(TM) i5-4570 CPU @ 3.20GHz 3.20 GHz |
| --- | --- |
| Оперативная память | 16,0 ГБ (доступно: 15,9 ГБ) |
| ОС | Windows 10 Pro |
| WSL2 | Ubuntu 20.04 LTS |

**Результаты измерения времени**

| **Номер замера** | № **1** | № **2** | № **3** | № **4** | № **5** | **AVG** |
| --- | --- | --- | --- | --- | --- | --- |
| **real** | 14.996s | 17.084s | 21.156s | 19.501s | 19.629s | **18.473s** |
| **user** | 14.974s | 17.052s | 20.896s | 19.491s | 19.618s | **18.406** |
| **sys** | 0.020s | 0.030s | 0.260s | 0.010s | 0.010s | **0.066s** |

**Таблица №1:** Измерение времени выполнения программы производится при помощи команды time. В последнем столбце указано средние арифметические показателей времени выполнения программы.

| **Номер замера** | **rdtsc (ticks)** | **rdtsc (s)** |
| --- | --- | --- |
| № **1** | 47870691233 | 14.95959 |
| № **2** | 54538945956 | 17.04342 |
| № **3** | 67539058030 | 21.10595 |
| № **4** | 62253141743 | 19.45410 |
| № **5** | 62663340226 | 19.58229 |
| **AVG** |
 | 18.42907 |

**Таблица №2:** Измерение времени выполнения программы производится при помощи функции rdtsc, в итоге программа выдает ~ число тиков между началом и концом программы.

Во втором столбце указано число тактов процессора за время выполнения программы. В последнем столбце указано время в секундах, полученное делением на тактовую частоту процессора, равную 3.2 GHz.

_Результаты экспериментов, занесённые в обе таблицы получены во время одних и тех же измерений._

**Способы ускорения выполнения программы**

Начнем с оптимизации алгоритма решения задачи: Заметим, что сложение элементов в одном и том же столбце матрицы C и в одной и той же взятой строке матрицы B происходит SIZE число раз. Улучшим алгоритм подсчитав каждую из 2\*SIZE сумм заранее и запомнив эти значения в дополнительные массивы. Далее используем эти суммы в последующих подсчетах. В итоге оценка сложности алгоритма снизится с O(n^3) до O(n^2) при незначительных накладных расходах.

| **Номер замера** | № **1** | № **2** | № **3** | № **4** | № **5** | **AVG** |
| --- | --- | --- | --- | --- | --- | --- |
| **real** | 0.047s | 0.051s | 0.048s | 0.049s | 0.050s | **0.049s** |
| **user** | 0.035s | 0.041s | 0.049s | 0.030s | 0.040s | **0.039s** |
| **sys** | 0.012s | 0.010s | 0.000s | 0.020s | 0.010s | **0.010s** |
| **rdtsc (s)** | 0.0472 | 0.0496 | 0.0451 | 0.0482 | 0.0491 | **0.04784** |

**Таблица №2:** Результаты показателей после оптимизации алгоритма

В продолжение банкета используем опцию компилятора gcc: –O2. Как известно, при стандартной компиляции программы на языке Си применяется опция –O0, относительно слабо оптимизирующая, однако опции (-O1, -O2, -O3) позволяют значительно ускорить выполнение программы:

| **Номер замера** | № **1** | № **2** | № **3** | № **4** | № **5** | **AVG** |
| --- | --- | --- | --- | --- | --- | --- |
| **real** | 0.023s | 0.023s | 0.023s | 0.022s | 0.022s | **0.023s** |
| **user** | 0.011s | 0.022s | 0.011s | 0.011s | 0.015s | **0.014s** |
| **sys** | 0.012s | 0.000s | 0.012s | 0.011s | 0.008s | **0.008s** |
| **rdtsc (s)** | 0.0214 | 0.0213 | 0.0217 | 0.0203 | 0.0210 | **0.0211** |

**Таблица №3:** Результаты показателей после применения опции –O2

**Выводы**

Как видим, при решении задач с большим числом арифметических операций полезно находить и использовать оптимальные алгоритмы. В рассмотренном примере такая оптимизация позволила ускорить выполнение программы в ~300-500 раз. А применяя опцию компилятора удалось добиться увеличения скорости еще в 2 раза.

Таким образом, ускорили среднее выполнение программы на 3 порядка по сравнению с изначальным результатом.