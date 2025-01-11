% Consider a set of non-null natural numbers represented as a list.
% Determine all the possible ways of writing a number N as a sum of elements from this list.

sumList([], 0).
sumList([H|T], S):-
    sumList(T, S1),
    S is S1 + H.

subset([], []).
subset([H|T], [H|R]):-
    subset(T, R).
subset([_|T], R):-
    subset(T, R).

isSumEqual(S, N):-
    sumList(S, Sum),
    Sum =:= N.

allForOne(N, L, Res):-
    findall(S, (subset(L, S), isSumEqual(S, N)), Res).
    
