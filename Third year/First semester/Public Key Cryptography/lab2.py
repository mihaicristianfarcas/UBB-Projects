import math


def primes_upto(limit: int):
    """Return a list of all primes ≤ limit by trial division."""
    primes = []
    for n in range(2, limit + 1):
        is_prime = True
        for p in primes:
            if p * p > n:
                break
            if n % p == 0:
                is_prime = False
                break
        if is_prime:
            primes.append(n)
    return primes


def factor_squarefree(n: int, prime_list):
    """Return a list of distinct prime factors of n.
       If n contains a squared prime, return None (not square‑free)."""
    factors = []
    temp = n
    for p in prime_list:
        if p * p > temp:
            break
        cnt = 0
        while temp % p == 0:
            temp //= p
            cnt += 1
        if cnt > 0:
            if cnt > 1:           # p^2 divides n → not square‑free
                return None
            factors.append(p)
    # the remaining part is a prime > sqrt(original n)
    if temp > 1:
        factors.append(temp)
    return factors                # distinct primes only


def is_carmichael(n: int, prime_list) -> bool:
    """Apply Korselt's criterion to n (n > 2)."""
    factors = factor_squarefree(n, prime_list)
    if factors is None:                 # not square‑free
        return False
    if len(factors) < 3:                # needs ≥3 distinct primes
        return False
    # check (p‑1) | (n‑1) for every factor
    for p in factors:
        if (n - 1) % (p - 1) != 0:
            return False
    return True


def carmichael_numbers(bound: int):
    """Generate all Carmichael numbers < bound."""
    # we only need primes up to sqrt(bound) for factoring
    primes = primes_upto(int(math.isqrt(bound)) + 1)
    result = []
    for n in range(2, bound):
        if is_carmichael(n, primes):
            result.append(n)
    return result


if __name__ == "__main__":
    B = 200_000
    carmichael_numbers = carmichael_numbers(B)
    print(f"Carmichael numbers < {B}:")
    print(carmichael_numbers)
    print("Count:", len(carmichael_numbers))
