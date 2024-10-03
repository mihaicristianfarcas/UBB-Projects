# Solve the problem from the second set here - 7
# Determine the twin prime numbers p1 and p2 immediately larger than the given non-null natural number n.
# Two prime numbers p and q are called twin if q - p = 2.

def is_prime(n: int) -> bool:
    if n < 2:
        return False
    i = 2
    while i*i <= n:
        if n % i == 0:
            return False
        i += 1
    return True

def twins(n :int):
    p1 = p2 = n + 1
    while True:
        if is_prime(p1):
            while not is_prime(p2):
                p2 += 1
            if p2-p1 == 2:
                print("Twin prime numbers:", p1, p2)
                break
            else:
                p1 = p2
                p2 += 2
        else:
            p1 += 1


number = int(input("Enter number:"))
twins(number)
