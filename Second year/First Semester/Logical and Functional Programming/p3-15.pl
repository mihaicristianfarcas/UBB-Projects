% Find all decompositions of N as sums of consecutive numbers

decompose(N, AllSolutions) :-
    findall(Sequence,
            (between(1, N, Start),
             decompose_from(Start, N, Sequence)),
            AllSolutions).

decompose_from(Start, Target, [Start]) :-
    Start =:= Target.

decompose_from(Start, Target, [Start|Rest]) :-
    Start < Target,
    Next is Start + 1,
    RemainingSum is Target - Start,
    RemainingSum > 0,
    decompose_from(Next, RemainingSum, Rest).

% ?- decompose(15, Solutions).