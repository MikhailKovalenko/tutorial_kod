import math
from typing import Tuple, Iterator, Iterable, Optional

def continuous_decomposition(x: int, y: int)-> Iterator[int]:# непрерывное разложение усложено так как для больших чисел
    while y:
        a = x // y
        yield a
        x, y = y, x - a * y

def suitable_fractions(arr_suitable_fractions: Iterable[int]) -> Iterator[Tuple[int, int]]:# подходящие дроби усложено так как для больших чисел
    n0, d0 = 0, 1
    n1, d1 = 1, 0
    for q in arr_suitable_fractions:
        n = q * n1 + n0
        d = q * d1 + d0
        yield n, d
        n0, d0 = n1, d1
        n1, d1 = n, d

def attack(e: int, n: int) -> Optional[int]:
    c_d = continuous_decomposition(e, n)
    c_f = suitable_fractions(c_d)
    print("хотите увидеть непрерывное разложение для e/m, введите: Y в обратном случае N")
    if input() == "Y":
        print(f"непрерывное разложение: {list(continuous_decomposition(e, n))}")
    print("хотите увидеть подходящие дроби для e/m, введите: Y в обратном случае N")
    if input() == "Y":
        print(f"подходящие дроби: {list(suitable_fractions(continuous_decomposition(e, n)))}")
    for k, d in c_f:
        if k>0 :
            phi = e * d // k
            x = n - phi + 1  # p+q замечание 2 атака винера
            if x % 2 == 0 and math.isqrt((x // 2) ** 2 - n)**2 ==(x // 2) ** 2 - n:  # y -> (p-q)/2
                x = x // 2
                y = math.isqrt(x ** 2 - n)
                p = x + y
                q = x - y
                return p,q,d
        else: continue
    return None

contin =True
while (contin):
    e = int(input("Введите открытую экспоненту (должна быть меньше модуля)"))
    m = int(input("Введите модуль"))
    p,q,d=attack(e,m)
    fi = (p-1)*(q-1)    # ed =1 (mod fi) -> ed = 1 + k*fi
    print(f"параметры шифра: \np = {p} \nq = {q} \nd = {d} \nfi = {fi} \ne = {e} \nm = {m}")
    print("хотите дальше использовать программу нажмите Y в обратном случае N")
    contin = True if input() == "Y" else False

