% TEST PROLOG

% is_list()
% atomic()
% number()


% nrOcc(l1, l2, ..., ln, N) = 0, n = 0
%							= 1 + nrOcc(l2, l3, ..., ln), l1 =:= N
%							= nrOcc(l2, l3, ..., ln), l1 \= N
% nrOcc(L - list, N - number, C - count)
% flow model (i, i, o)

nrOcc([], _, 0).

nrOcc([H|T], N, C):-
    nrOcc(T, N, C1),
    H == N,
    C is C1 + 1.

nrOcc([H|T], N, C):-
    H =\= N,
    nrOcc(T, N, C).


% union(l1, l2, ..., ln, c1, c2, ..., cm) = [], n = 0, m = 0
%						 = l1 U union(l2, l3, ..., ln, c1, c2, ..., cm), l1 not in c1, c2, ..., cm
%						 = union(l2, l3, ..., ln, c1, c2, ..., cm), l1 in c1, c2,

union([], L, L).

union([H|T], L, [H|R]) :-
    nrOcc(L, H, C),
    C =:= 0,
    union(T, L, R).

union([H|T], L, R) :-
    nrOcc(L, H, C),
    C =\= 0,
    union(T, L, R).


% remDuplicates(l1, l2, ..., ln, c1, c2, ..., cm) = [], n = 0
% 							 				  = l1 U remDuplicates(l2, l3, ..., ln), nrOcc(
% 							 				  = remDuplicates(l2, l3, ..., ln), nrOcc(c1, c2, ..., cm, l1) = 2
% remDuplicates(L - list, C - copy list, R - result list)
% flow model (i, i, o)


remDuplicates([], _, []).
remDuplicates([H|T], L, [H|R]) :-
    nrOcc(L, H, C),
    C =:= 1, 
    remDuplicates(T, L, R).
remDuplicates([H|T], L, R) :-
    nrOcc(L, H, C),
    C =\= 1,
    remDuplicates(T, L, R).


mainUnion(L, M, R):- 
    union(L, M, X),
    remDuplicates(X, X, R).