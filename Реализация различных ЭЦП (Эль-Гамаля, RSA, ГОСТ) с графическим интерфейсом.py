import hashlib
from tkinter import *
from random import *
import sympy

#region Глобальные переменные, используются в функциях
#region rsa  global
p = 1
q = 1
d = 1
m = 1
L = 1
e = 1
x = 1
s = 1
#endregion
#region gost global
mod = 11
a = 4
b = 7
q = 1
G = [1, 1]
#endregion
#endregion
# region эцп гост
def gost():
    # Вычисление порядка G (Просто меняет глобальную переменную, ничего не возвращает)
    def point():
        global mod, a, b, G
        x = randint(0, mod - 1)
        for i in range(0, mod - 1):  # перебираю всевозможные x
            y = (((x + i) % mod) ** 3 + a * ((x + i) % mod) + b) % mod  # нахожу y^2
            if (y ** (int((mod - 1) / 2))) % mod == 1:
                for j in range(1, mod):
                    if (j ** 2) % mod == y:
                        G = [(x + i) % mod, j]
                        equalq()
                        if sympy.isprime(q) == True and q > 3:
                            return (1)
        infolabel1.config(text="Измените параметры кривоЙ, иначе")
        infolabel2.config(text="Программа будет работать некорректно")
        return (0)

    def equalq():
        global q
        A = G.copy()
        q = 1
        while (A != ['Q', 'Q']):
            q = q + 1
            A = Sum_A_and_B(A, G)

    # Проверка существует ли точка на элептической кривой
    def check(A):
        return (bool(((A[1] ** 2) % mod) == (((A[0] ** 3) + a * A[0] + b) % mod)))  # №#

    # Cумма точек A и B
    def Sum_A_and_B(A1, A2):
        if (A1 == [0, 0] or A1 == ['Q', 'Q']):
            return A2
        elif (A2 == [0, 0] or A2 == ['Q', 'Q']):
            return A1
        else:
            if (A1 != A2):
                if (A1[0] == A2[0]): return ['Q', 'Q']
                if (A1[0] != A2[0]):
                    k = ((A2[1] - A1[1]) * pow(A2[0] - A1[0], -1, mod)) % mod
                    x3 = (k ** 2 - (A1[0] + A2[0])) % mod
                    y3 = (k * (A1[0] - x3) - A1[1]) % mod
                    A3 = [x3, y3]
                    return A3
            if (A1 == A2):
                if (A1[1] == 0): return ['Q', 'Q']
                if (A1[1] != 0):
                    k = ((3 * A1[0] ** 2 + a) * pow(2 * A1[1], -1, mod)) % mod
                    x3 = (k ** 2 - (A1[0] + A2[0])) % mod
                    y3 = (k * (A1[0] - x3) - A1[1]) % mod
                    A3 = [x3, y3]
                    return A3

    # Умножение точки A на n
    def n_A_quickly(A1, n):  # быстрый метод
        arr = ((bin(int(n))).replace('0b', ''))  # преобразование числа х в бинарный вид и удаление 0b вначале
        alpha = A1.copy()
        for i in range(1, len(arr)):
            if arr[i] == '1':
                alpha = Sum_A_and_B((Sum_A_and_B(alpha, alpha)), A1)
            else:
                alpha = Sum_A_and_B(alpha, alpha)
        return (alpha)

    # region создание виджетов(Писец непонятно и некрасиво)
    window = Tk()
    window.title("Электронная цифровая подпись Гост")
    infolabel1 = Label(window, fg='#008000', text='Оставте точку G незаполненной', width=59, height=1, bg='#000000')
    infolabel2 = Label(window, fg='#008000', text='Если желаете, что бы она нашлась автоматически', width=59, height=1,
                       bg='#000000')
    infolabel1.place(x=25, y=10)
    infolabel2.place(x=25, y=30)

    infolabel3 = Label(window, text='mod - простое число.')
    infolabel3.place(x=25, y=230)
    infolabel4 = Label(window, text='a - небольшое целое число.')
    infolabel4.place(x=25, y=250)
    infolabel4 = Label(window, text='b - целое число')
    infolabel4.place(x=25, y=270)
    infolabel5 = Label(window, text='k - секретный ключ, целое число')
    infolabel5.place(x=25, y=290)
    infolabel6 = Label(window, text='K - открытый ключ целое число')
    infolabel6.place(x=25, y=310)
    infolabel7 = Label(window, text='Остальные параметры создаются автоматически, либо вводить уже известные')
    infolabel7.place(x=25, y=330)
    # ----------------------------------
    # Параметр mod
    Param_mod = Entry(window, width=7, bg='#dcdcdc')
    labelmod = Label(window, text='mod')
    labelmod.place(x=25, y=65)
    Param_mod.place(x=60, y=65)
    # Параметр а
    Param_a = Entry(window, width=7, bg='#dcdcdc')
    labela = Label(window, text='а')
    labela.place(x=120, y=65)
    Param_a.place(x=135, y=65)
    # Параметр b
    Param_b = Entry(window, width=7, bg='#dcdcdc')
    labelb = Label(window, text='b')
    labelb.place(x=195, y=65)
    Param_b.place(x=210, y=65)
    # Точка G
    Param_Gx = Entry(window, width=5, bg='#dcdcdc')
    Param_Gy = Entry(window, width=5, bg='#dcdcdc')
    labelG = Label(window, text='G (')
    labelznak1 = Label(window, text=';')
    labelznak2 = Label(window, text=')')
    labelG.place(x=270, y=65)
    labelznak1.place(x=338, y=65)
    labelznak2.place(x=390, y=65)
    Param_Gx.place(x=292, y=65)
    Param_Gy.place(x=345, y=65)

    Param_q = Entry(window, width=7, bg='#dcdcdc')
    labelq = Label(window, text='q')
    labelq.place(x=405, y=65)
    Param_q.place(x=420, y=65)
    # =================================
    # Доп поля ввода
    LabelSign = Label(window, text='k')
    EntrySign = Entry(window, width=7, bg='#dcdcdc')
    labelProofK1 = Label(window, text='K (')
    labelProofK2 = Label(window, text=';')
    labelProofK3 = Label(window, text=')')
    EntryProofKx = Entry(window, width=5, bg='#dcdcdc')
    EntryProofKy = Entry(window, width=5, bg='#dcdcdc')
    labelProofS1 = Label(window, text='S1')
    labelProofS2 = Label(window, text='S2')
    EntryS1 = Entry(window, width=7, bg='#dcdcdc')
    EntryS2 = Entry(window, width=7, bg='#dcdcdc')
    ButtonSignnext = Button(window, text='Далее', command=lambda: Signnext())
    ButtonProofnext = Button(window, text='Далее', command=lambda: Proofnext())
    LabelX = Label(window, text='x')
    LabelH = Label(window, text='x')
    EntryXH = Entry(window, width=7, bg='#dcdcdc')

    #  endregion
    # =================================
    # Функции кнопок
    # Поля ввода для подписи
    def SignButton():
        if Preparation():
            infolabel1.config(text="Введите Данные")
            infolabel2.config(text="")
            LabelSign.place(x=150, y=110)
            EntrySign.place(x=165, y=110)
            ButtonSignnext.place(x=345, y=190)
            LabelX.place(x=150, y=150)
            EntryXH.place(x=165, y=150)

    # Поля ввода для проверки
    def ProofButton():
        if Preparation():
            infolabel1.config(text="Введите Данные")
            infolabel2.config(text="")
            labelProofK1.place(x=150, y=110)
            labelProofK2.place(x=218, y=110)
            labelProofK3.place(x=270, y=110)
            EntryProofKx.place(x=172, y=110)
            EntryProofKy.place(x=225, y=110)
            labelProofS1.place(x=150, y=150)
            EntryS1.place(x=170, y=150)
            labelProofS2.place(x=225, y=150)
            EntryS2.place(x=245, y=150)
            ButtonProofnext.place(x=345, y=190)
            LabelH.place(x=150, y=190)
            EntryXH.place(x=165, y=190)

    # Сокрытие полей ввода для проверки и подписи
    def Preparation():
        LabelSign.place_forget()
        EntrySign.place_forget()
        labelProofK1.place_forget()
        labelProofK2.place_forget()
        labelProofK3.place_forget()
        EntryProofKx.place_forget()
        EntryProofKy.place_forget()
        labelProofS1.place_forget()
        labelProofS2.place_forget()
        EntryS1.place_forget()
        EntryS2.place_forget()
        ButtonSignnext.place_forget()
        ButtonProofnext.place_forget()
        LabelX.place_forget()
        LabelH.place_forget()
        EntryXH.place_forget()
        global mod, a, b, G, q
        if (mod != int(Param_mod.get()) or a != int(Param_a.get()) or b != int(Param_b.get()) or str(G[0]) !=
            Param_Gx.get()) or (str(G[1]) != Param_Gy.get()):
            mod = int(Param_mod.get())
            a = int(Param_a.get())
            b = int(Param_b.get())
            if (4 * a * a + 27 * b * b) == 0:
                infolabel1.config(text="Некорректные данные")
                infolabel2.config(text="")
                return (0)
            if Param_Gx.get() == '' or Param_Gy.get() == '':
                if point():
                    Param_Gx.delete(0, 'end')
                    Param_Gx.insert(0, G[0])
                    Param_Gy.delete(0, 'end')
                    Param_Gy.insert(0, G[1])
                    Param_q.delete(0, 'end')
                    Param_q.insert(0, str(q))
                else:
                    return (0)
            else:
                G = [(int(Param_Gx.get()) % mod), (int(Param_Gy.get()) % mod)]
                if (check(G)):
                    equalq()
                    Param_q.delete(0, 'end')
                    Param_q.insert(0, str(q))
                    if sympy.isprime(q) == False:
                        infolabel1.config(text="Некорректно выбрана точка G")
                        infolabel2.config(text="Порядок точки G не является простым числом")
                        return (0)
                else:
                    infolabel1.config(text="Точка G не принадлежит кривой")
                    infolabel2.config(text="")
                    return (0)
        return (check(G))

    def hash(x):# хеширование
        h = x.encode('utf-8')  # Преобразование строки в байты
        hash_object = hashlib.sha256(h)  # Шифрование при помощи стандарта sha256
        hex_dig = hash_object.hexdigest()  # Перевод в 16-ную систему исчисления
        return int(hex_dig, 16)

    # Функция проверки
    def Signnext():
        x = hash(EntryXH.get())
        k = int(EntrySign.get())
        while True:
            while True:
                r = randint(2, q - 1)  # Cеансовый ключ
                K = n_A_quickly(G, k)  # Открытый ключ
                P = n_A_quickly(G, r)
                S1 = P[0] % q
                if S1 != 0:
                    break
            S2 = (r * x + S1 * k) % q
            if S2 != 0:
                break
        infolabel1.config(text=(f"Открытый ключ: К=({K[0]};{K[1]})"))
        infolabel2.config(text=('(S1,S2)=(', S1, ',', S2, ')'))
        # region Автозаполнение полей ввода
        EntryS1.delete(0, 'end')
        EntryS2.delete(0, 'end')
        EntryS1.insert(0, S1)
        EntryS2.insert(0, S2)
        EntryProofKy.delete(0, 'end')
        EntryProofKy.insert(0, K[1])
        EntryProofKx.delete(0, 'end')
        EntryProofKx.insert(0, K[0])
        #  endregion

    def Proofnext():
        S1 = int(EntryS1.get())
        S2 = int(EntryS2.get())
        pr = bool(0)
        if (S1 < q and S1 > 1 and S2 < q and S2 > 1):
            K = [int(EntryProofKx.get()), int(EntryProofKy.get())]
            x = hash(EntryXH.get())
            u1 = -S1 * (pow(x, -1, q)) % q
            u2 = (S2 * pow(x, -1, q)) % q
            Q = Sum_A_and_B(n_A_quickly(K, u1), n_A_quickly(G, u2))
            if S1 % q == Q[0] % q:
                infolabel1.config(text='Подпись подтверждена')
                infolabel2.config(text='')
                pr = 1
        if pr == 0:
            infolabel1.config(text='Подпись отвергается')
            infolabel2.config(text='')

    # Кнопки
    # =================================
    ButtonProof = Button(window, text='Проверить', command=lambda: ProofButton())
    ButtonSign = Button(window, text='Подписать', command=lambda: SignButton())
    ButtonProof.place(x=25, y=130)
    ButtonSign.place(x=25, y=190)
    window.geometry('520x380')
# endregion
#region эцп эль-гамаля
def EL_Gamal():
#region интерфейс
    window_Gamal = Tk()
    window_Gamal.title("Электронная цифровая подпись Эль-Гамаля")
    window_Gamal.geometry('540x300')
    infolabel1 = Label(window_Gamal, fg='#008000', text='Введите значения параметров', width=59, height=1, bg='#000000')
    infolabel2 = Label(window_Gamal, fg='#008000', text='', width=59, height=1,bg='#000000')

    infolabel1.place(x=25,y=10)
    infolabel2.place(x=25, y=30)

    infolabel3 = Label(window_Gamal, text='p - простое число. Заполнять при создании подписи.')
    infolabel3.place(x=25, y=145)
    infolabel4 = Label(window_Gamal, text='q - первообразный корень по модулю p. Заполнять при создании подписи.')
    infolabel4.place(x=25, y=165)
    infolabel4 = Label(window_Gamal, text='key - секретный ключ. Заполнять при создании подписи.')
    infolabel4.place(x=25, y=185)
    infolabel7 = Label(window_Gamal, text='u - открытый ключ, вычисляется так: u=g^key(mod p). Заполнять при проверке подписи.')
    infolabel7.place(x=25, y=205)
    infolabel5 = Label(window_Gamal, text='x - исходный текст. Заполнять при создании/проверке подписи.')
    infolabel5.place(x=25, y=225)
    infolabel6 = Label(window_Gamal, text='s1,s2 - подпись. Заполнять при проверке подписи.')
    infolabel6.place(x=25, y=245)

    #region кнопки
    # Параметр S2
    Param_S2 = Entry(window_Gamal, width=7)
    labelS2 = Label(window_Gamal, text='s2')
    labelS2.place(x=320, y=65)
    Param_S2.place(x=335, y=65)
    # Параметр S1
    Param_S1 = Entry(window_Gamal, width=7)
    labelS1 = Label(window_Gamal, text='s1')
    labelS1.place(x=380, y=65)
    Param_S1.place(x=395, y=65)
    # Параметр p
    Param_p = Entry(window_Gamal, width=7)
    labelp = Label(window_Gamal, text='p')
    labelp.place(x=20, y=65)
    Param_p.place(x=35, y=65)
    # Параметр q
    Param_q = Entry(window_Gamal, width=7)
    labelq = Label(window_Gamal, text='q')
    labelq.place(x=80, y=65)
    Param_q.place(x=95, y=65)
    # Параметр key
    Param_key = Entry(window_Gamal, width=7)
    labelkey = Label(window_Gamal, text='key')
    labelkey.place(x=140, y=65)
    Param_key.place(x=165, y=65)
    # Параметр x
    Param_x = Entry(window_Gamal, width=7)
    labelx = Label(window_Gamal, text='x')
    labelx.place(x=260, y=65)
    Param_x.place(x=275, y=65)
    # Параметр u
    Param_u = Entry(window_Gamal, width=7)
    labelu = Label(window_Gamal, text='u')
    labelu.place(x=200, y=65)
    Param_u.place(x=215, y=65)

    ButtonCreateGamal = Button(window_Gamal, text='Создать подпись',command=lambda:Create_Gamal())
    ButtonCreateGamal.place(x=25, y=95)

    ButtonFindGamal = Button(window_Gamal, text='Проверить подпись', command=lambda: Find_Gamal())
    ButtonFindGamal.place(x=250, y=95)
    #endregion
    def hash(x):  # хеширование
        h = x.encode('utf-8')  # Преобразование строки в байты
        hash_object = hashlib.sha256(h)  # Шифрование при помощи стандарта sha256
        hex_dig = hash_object.hexdigest()  # Перевод в 16-ную систему исчисления
        return int(hex_dig, 16)
    def prost(r, p1):
        while (r and p1):
            if (r>p1):
                r %= p1
            else:
                p1 %= r
        if (p1+r == 1):
            return 1
        else:
            return 0
    def random_r(p1,l):
        while (l==0):
            r = randint(1, p1)
            l=prost(r, p1)
        return r

    def Create_Gamal():
        p = int(Param_p.get())  # открытый ключ
        q = int(Param_q.get())  # открытый ключ
        key = int(Param_key.get())  # секретный ключ
        H = hash(Param_x.get())%p  # открытый ключ
        r=random_r(p-1,0)
        r1 = pow(r, -1, p - 1)
        s1=q**r % p
        s2=((H-(s1*key))*(r1)) % (p-1)
        infolabel1.config(text=f"Цифровая подпись El-Gamal - (x, (S1,s2)): ({Param_x.get()},({s1},{s2}))")

    def Find_Gamal():
        p = int(Param_p.get())  # открытый ключ
        q = int(Param_q.get())  # открытый ключ
        key = int(Param_key.get())  # секретный ключ
        H = hash(Param_x.get())%p  # открытый ключ
        u = int(Param_u.get()) # открытый ключ
        s1 = int(Param_S1.get())
        s2 = int(Param_S2.get())
        if (0<s1<p) and (0<s2<p-1):
            if ((((u**s1)*(s1**s2))%p)==((q**H)%p)):
                infolabel1.config(text="Подпись принята")
            else: infolabel1.config(text="Подпись не принята")
        else: infolabel1.config(text="Подпись незаконна")
#endregion
# region эцп rsa
def rsa():
    def Create_Rsa():
        enter()
        h = x.encode('utf-8')  # Преобразование строки в байты
        hash_object = hashlib.sha256(h)  # Шифрование при помощи стандарта sha256
        hex_dig = hash_object.hexdigest()  # Перевод в 16-ную систему исчисления
        U = int(hex_dig, 16)  # Перевод в 10-ную систему исчисления
        S = pow(U, d, m)  # ЭЦП
        infolabel1.config(text = f"Цифровая подпись RSA - (x, S): ({x},{S})")
    def Find_Rsa():
        h = x.encode('utf-8')  # Преобразование строки в байты
        hash_object = hashlib.sha256(h)  # Шифрование при помощи стандарта sha256
        hex_dig = hash_object.hexdigest()  # Перевод в 16-ную систему исчисления
        U = int(hex_dig, 16)  # Хеш сообщения x
        S = int(Param_s.get())
        if pow(S, e, m) == U % m:  # Проверка подписи
            infolabel1.config(text ="Верная подпись!")
        else:
            infolabel1.config(text ="Неверная подпись!")
    window_rsa = Tk()
    window_rsa.title("Электронная цифровая подпись RSA")
    window_rsa.geometry('520x300')
    infolabel1 = Label(window_rsa, fg='#008000', text='Введите значения параметров', width=59, height=1, bg='#000000')
    infolabel2 = Label(window_rsa, fg='#008000', text='', width=59, height=1,bg='#000000')

    infolabel1.place(x=25,y=10)
    infolabel2.place(x=25, y=30)
    #Описание параметров, ИНФО
    infolabel3 = Label(window_rsa, text='p, q - простые числа. Заполнять при создании подписи.')
    infolabel3.place(x=25, y=155)
    infolabel4 = Label(window_rsa, text='d - секретный ключ. Заполнять при создании подписи.')
    infolabel4.place(x=25, y=175)
    infolabel4 = Label(window_rsa, text='e - открытый ключ, обратный к d по модулю fi(mod). Заполнять при проверке подписи.')
    infolabel4.place(x=25, y=195)
    infolabel5 = Label(window_rsa, text='x - исходный текст. Заполнять при создании/проверке подписи.')
    infolabel5.place(x=25, y=215)
    infolabel6 = Label(window_rsa, text='s - подпись. Заполнять при проверке подписи.')
    infolabel6.place(x=25, y=235)
    #region кнопки
    # Параметр S
    Param_s = Entry(window_rsa, width=7)
    labels = Label(window_rsa, text='s')
    labels.place(x=415, y=65)
    Param_s.place(x=430, y=65)
    # Параметр p
    Param_p = Entry(window_rsa, width=7)
    labelp = Label(window_rsa, text='p')
    labelp.place(x=20, y=65)
    Param_p.place(x=35, y=65)
    # Параметр q
    Param_q = Entry(window_rsa, width=7)
    labelq = Label(window_rsa, text='q')
    labelq.place(x=80, y=65)
    Param_q.place(x=95, y=65)
    # Параметр d
    Param_d = Entry(window_rsa, width=7)
    labeld = Label(window_rsa, text='d')
    labeld.place(x=235, y=65)
    Param_d.place(x=250, y=65)
    # Параметр x
    Param_x = Entry(window_rsa, width=7)
    labelx = Label(window_rsa, text='x')
    labelx.place(x=355, y=65)
    Param_x.place(x=370, y=65)
    # Параметр mod
    Param_mod = Entry(window_rsa, width=7)
    labelmod = Label(window_rsa, text='mod')
    labelmod.place(x=140, y=65)
    Param_mod.place(x=175, y=65)
    # Параметр e
    Param_e = Entry(window_rsa, width=7)
    labele = Label(window_rsa, text='e')
    labele.place(x=295, y=65)
    Param_e.place(x=310, y=65)

    ButtonCreateRsa = Button(window_rsa, text='Создать подпись RSA',command=lambda:Create_Rsa())
    ButtonCreateRsa.place(x=25, y=125)

    ButtonFindRsa = Button(window_rsa, text='Проверить подпись RSA', command=lambda: Find_Rsa())
    ButtonFindRsa.place(x=250, y=125)
#endregion
    # Электронная цифровая подпись RSA
    import math, hashlib
    def enter():
        global p,q,d,m,L,e,x
        p = int(Param_p.get())# Простое число
        q = int(Param_q.get())# Простое число
        d = int(Param_d.get())# Секретная экспонента
        x = Param_x.get()# Сообщение от пользователя
        m = int(Param_mod.get())  # Модуль m
        L = (p - 1) * (q - 1)  # Функция эйлера для простых чисел

        while math.gcd(d, L) > 1:
            infolabel1.config(text="Измените значение секретного параметра d")# Проверка на нод
            window_rsa.mainloop()
            d = int(Param_d.get())# Секретная экспонента
        e = int(Param_e.get())
# endregion
def info():
    from PIL import Image, ImageTk
    img = Image.open('Безымянный.png')
    img.show()
MainWin= Tk()
MainWin.title("Примеры современных ЭЦП")
Button_rsa=Button(MainWin,text='Эцп rsa',command=lambda:rsa())
Button_gamal=Button(MainWin,text='Эцп Эль-Гамаля',command=lambda:EL_Gamal())
Button_gost=Button(MainWin,text='Эцп Гост',command=lambda:gost())
Button_info=Button(MainWin,text='Инфо',command=lambda:info())
Button_rsa.place(x=125,y=60)
Button_gamal.place(x=125,y=120)
Button_gost.place(x=125,y=180)
Button_info.place(x=0,y=0)
MainWin.geometry('330x250')
MainWin.mainloop()



