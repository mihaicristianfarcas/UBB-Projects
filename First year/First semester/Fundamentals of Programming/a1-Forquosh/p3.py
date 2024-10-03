# Solve the problem from the third set here - 13
# Determine the n-th element of the sequence 1,2,3,2,5,2,3,7,2,3,2,5,...
# Obtained from the sequence of natural numbers by replacing composed numbers with their prime divisors
# Without memorizing the elements of the sequence.

def prime(n :int) -> int:
    if n == 1:
        return 1
    else:
        cnt = 1
        nr = 1
        i = 2
        while True:
            copy = i
            d = 2 ; p = 0
            while i > 1:
                while i % d == 0:
                    i = int(i/d)
                    p += 1
                if p != 0:
                    nr += 1
                    cnt = d
                d += 1
                if d * d > i:
                    d = i
                if nr == n:
                    return cnt
            i = copy + 1

print("Enter you number:")
number = int(input(">"))
print("The corresponding number is:", prime(number))


