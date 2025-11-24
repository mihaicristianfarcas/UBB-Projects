import argparse
import math
import random
from typing import Callable, List, Optional

IterFunction = Callable[[int, int], int]


def default_polynomial(x: int, n: int) -> int:
    return (x * x + 1) % n


def _iterate(f: IterFunction, value: int, modulus: int) -> int:
    return f(value, modulus) % modulus


def pollards_rho(
    n: int,
    f: Optional[IterFunction] | None,
    *,
    x0: int = 2,
    max_iterations: int = 100_000,
) -> Optional[int]:
    """
    Attempt to find a non-trivial divisor of n using Pollard's rho algorithm.

    Args:
        n: Composite integer to factor.
        f: Iteration function f(x, n). Defaults to x^2 + 1 (mod n).
        x0: Initial value for the sequence.
        max_iterations: Hard stop to avoid infinite loops.

    Returns:
        A non-trivial factor of n or None if the attempt fails.
    """
    if n <= 1:
        raise ValueError("Pollard's rho requires n >= 2")
    if n % 2 == 0:
        return 2

    func = f or default_polynomial
    x = x0 % n
    y = x
    d = 1

    for i in range(max_iterations):
        x = _iterate(func, x, n)
        y = _iterate(func, _iterate(func, y, n), n)
        d = math.gcd(abs(x - y), n)
        print(f"  Iteration {i}: x={x}, y={y}, d={d}")
        if d == 1:
            continue
        if d == n:
            return None
        return d
    return None


def is_probably_prime(n: int) -> bool:
    if n < 2:
        return False
    small_primes = (2, 3, 5, 7, 11, 13, 17, 19, 23, 29)
    for p in small_primes:
        if n == p:
            return True
        if n % p == 0:
            return False

    i = 5
    limit = math.isqrt(n)
    while i <= limit:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True


def factor_with_pollards_rho(
    n: int,
    f: Optional[IterFunction] = None,
    *,
    x0: int = 2,
    max_iterations: int = 100_000,
    attempt_limit: int = 25,
    rng_seed: Optional[int] = None,
) -> List[int]:
    """
    Factor n by recursively applying Pollard's rho algorithm.

    Raises RuntimeError if a factor cannot be found with the allotted attempts.
    """
    if n < 2:
        raise ValueError("Cannot factor numbers < 2")

    func = f or default_polynomial
    rng = random.Random(rng_seed)
    factors: List[int] = []

    def _factor(m: int, x_start: int) -> None:
        print(f"Attempting to factor: {m}")
        if m == 1:
            return
        if is_probably_prime(m):
            print(f"Found prime factor: {m}")
            factors.append(m)
            return

        attempt = 0
        while attempt < attempt_limit:
            print(f"  Pollard's rho attempt {
                  attempt + 1} for n={m} with x0_seed={x_start}")
            seed = x_start if attempt == 0 else rng.randrange(2, m - 1)
            divisor = pollards_rho(
                m, func, x0=seed, max_iterations=max_iterations
            )
            if divisor is None or divisor == m:
                print(
                    f"  Pollard's rho failed to find a non-trivial divisor or found n for n={m} with seed={seed}")
                attempt += 1
                continue
            print(f"  Found non-trivial divisor: {divisor} for n={m}")
            _factor(divisor, seed)
            _factor(m // divisor, seed)
            return
        raise RuntimeError(f"Pollard's rho failed for n={m}")

    _factor(n, x0)
    factors.sort()
    return factors


def parse_iter_function(expr: str) -> IterFunction:
    """
    Convert a string into a callable iteration function.

    Example: "lambda x, n: (x*x + 3) % n"
    """
    allowed_globals = {"math": math}
    try:
        candidate = eval(expr, {"__builtins__": {}}, allowed_globals)
    except Exception as exc:  # pylint: disable=broad-except
        raise argparse.ArgumentTypeError(
            f"Unable to parse function: {expr}"
        ) from exc
    if not callable(candidate):
        raise argparse.ArgumentTypeError("Provided expression is not callable")
    return candidate


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Pollard's rho demonstration with customizable iteration "
        "function."
    )
    parser.add_argument(
        "n",
        type=int,
        help="Composite integer to factor.",
    )
    parser.add_argument(
        "--func",
        type=parse_iter_function,
        help="Python expression for f(x, n), e.g. \"lambda x, n: (x*x + 3) %% n\"",
    )
    parser.add_argument(
        "--x0",
        type=int,
        default=2,
        help="Initial value for the sequence (default: 2).",
    )
    parser.add_argument(
        "--max-iterations",
        type=int,
        default=100_000,
        help="Iteration cap per rho attempt (default: 100000).",
    )
    parser.add_argument(
        "--attempts",
        type=int,
        default=25,
        help="Maximum retries with different seeds (default: 25).",
    )
    parser.add_argument(
        "--rng-seed",
        type=int,
        help="Seed for the RNG used when new seeds are needed.",
    )
    args = parser.parse_args()

    try:
        factors = factor_with_pollards_rho(
            args.n,
            args.func,
            x0=args.x0,
            max_iterations=args.max_iterations,
            attempt_limit=args.attempts,
            rng_seed=args.rng_seed,
        )
    except Exception as exc:  # pylint: disable=broad-except
        print(f"Failed to factor {args.n}: {exc}")
        return

    print(f"Factors of {args.n}: {' * '.join(str(f) for f in factors)}")


if __name__ == "__main__":
    main()
