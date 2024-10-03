# Solve the problem from the first set here - 1
# Generate the first prime number larger than a given natural number n.
def is_prime(n: int) -> bool:
    if n < 2:
        return False
    i = 2
    while i * i <= n:
        if n % i == 0:
            return False
        i += 1
    return True

def first(n :int):
    while True:
        if is_prime(n):
            print("first prime number larger than n:", n)
            break
        else:
            n = n + 1


number = int(input("Enter number:"))
first(number)
