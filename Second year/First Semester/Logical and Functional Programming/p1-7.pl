% Problem 7

% a. Write a predicate to compute the intersection of two sets.
% inter(l1, l2, ..., ln, c1, c2, .., cm) = [] if n = 0, m = 0
%										 = l1 U inter(l2, ..., ln, c1, c2, .., cm) if l1 in c1, c2, ..., cm
%										 = inter(l2, ..., ln, c1, c2, ..., cm) if l1 not in c1, c2, ..., cm
% inter(L - list, C - list, R - result list)
% flow model (i, i, o)

inter([], _, []).
inter([H|T], C, [H|R]):-
    member(H, C),
    inter(T, C, R).

inter([_|T], C, R):-
    inter(T, C, R).

mainInter(L, C, R):-inter(L, C, R).

% b. Write a predicate to create a list (m, ..., n) of all integer numbers from the interval [m, n].
% integers(m, n) = [], if m > n
%				 = m U integers(m + 1, n), if m <= n
% integers(M - integer, N - integer, R - result list)
% flow model(i, i, o)

integer(M, N, []):-
    M > N.

integer(M, N, [M|R]):-
    M =< N,
    M1 is M + 1,
    integer(M1, N, R).

mainInteger(M, N, R):-integer(M, N, R).