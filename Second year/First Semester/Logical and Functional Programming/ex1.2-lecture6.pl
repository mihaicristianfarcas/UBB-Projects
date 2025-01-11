% Given a set represented as a list, find the subsets of even sum formed by odd numbers only.

% main predicate
evenSumOddNumbers(List, Subsets) :-
    include(is_odd, List, OddNumbers), % filter odd
    findall(S, (subset(OddNumbers, S), even_sum(S)), Subsets). % generate all sol.

% check if elem. is odd
is_odd(X) :-
    X mod 2 =:= 1.

% gen. subsets
subset([], []).
subset([H|T], [H|SubT]) :-
    subset(T, SubT).
subset([_|T], SubT) :-
    subset(T, SubT).

% check parity of subset
even_sum(List) :-
    sum_list(List, Sum),
    Sum mod 2 =:= 0.