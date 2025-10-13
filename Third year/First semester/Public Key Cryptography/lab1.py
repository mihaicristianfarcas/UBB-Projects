# Lab 1 - Greatest Common Divisor

import time

numbers = [(24, 36),
           (300, 576),
           (1234141234, 1985807238),
           (2**20, 4**9),
           (23, 5135),
           (351418020, 41241524),
           (141241, 351525152152),
           (2412152756, 31571857),
           (1111111, 22222222222),
           (0, 1)
           ]

# Consecutive substraction
def GCD1(a: int, b: int) -> int:
    
    while a != b and a > 0 and b > 0:
        if a > b:
            a = a - b
        else:
            b = b - a

    return b


# Modulo
def GCD2(a: int, b: int) -> int:

    while b != 0:
        r = a % b
        a = b
        b = r
    
    return a


# With Modulo Recursive
def GCD3(a: int, b: int) -> int:
    
    if b == 0:
        return a

    return GCD3(b, a % b)


for pair in numbers:
    a, b = pair[0], pair[1]
    
    print(f'Numbers: {a}, {b}')
    
    start = time.time()
    print(f'Consecutive substraction: {GCD1(a, b)}')
    end = time.time()
    print(f'This operation took {(end - start) } seconds')

    start = time.time()
    print(f'Modulo (Euclid): {GCD2(a, b)}')
    end = time.time()
    print(f'This operation took {(end - start) } seconds')
    
    start = time.time()
    print(f'Modulo (Recursive): {GCD3(a, b)}')
    end = time.time()
    print(f'This operation took {(end - start) } seconds')
    
    print()
