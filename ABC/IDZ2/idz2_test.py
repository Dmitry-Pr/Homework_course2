def func(x):
    return x ** 4 + 2 * x ** 3 - x - 1


acc = [0.001, 0.0001, 0.00000001]
for eps in acc:
    a = 0
    b = 1
    while True:
        c = (a + b) / 2
        if func(c) * func(a) < 0:
            b = c
        else:
            a = c
        if b - a <= eps:
            print(eps, (a + b) / 2)
            break
