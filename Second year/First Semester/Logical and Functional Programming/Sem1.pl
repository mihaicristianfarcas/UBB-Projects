% Multiply the elements of a list with a constant value.
% [1, 2, 3] ; k = 10 => [10, 20, 30]
% mulk(L - list, K - number, R - result list)
% flow model (i, i, o), (i, i, i) ; i = input / o = output

mulk([], _, []).
mulk([H|T], K, R):-
    H1 is H*K,
    mulk(T, K, TR),
    R=[H1|TR].

% mulk([1, 2, 3], 10, R).



% Append an element at the end of a list
% appe(L - list, K - number, R - result list)
% [1, 2, 3] ; e = 11 => [1, 2, 3, 11]
% flow model (i, i, o), (i, i, i) ; i = input / o = output

appn([], K, [K]).
appn([H|T], K, [H|TR]):-
    appn(T, K, TR).

% appe([1, 2, 3], 11, R).
% appe([1, 2, 3], 11, [1, 2, 3, 11]).



% Compute the sum of elements of a numerical list
% sumList(e1, e2...) = { 0, if n = 0 
%  						  e1 + sumList(e2, e3...), if n > 0
% sumList(L - list, S - number)
% flow model (i, o), (i, i)

sumList([], 0).
sumList([H|T], S):-
    sumList(T, TS),
    S is H+TS.

% sumList([1, 2, 3], R).
% trace, (sumList([1, 2, 3], R)).



% Compute the product of even elements in a numerical list
% prodList(L - list, P - number)
% flow model (i, o), (i ,i)

prodList([], 1).
prodList([H|T], P):-
    H mod 2 =\= 0,
    prodList(T, P).

prodList([H|T], P):-
    H mod 2 =:= 0,
    prodList(T, TP),
    P is H*TP.
    
% prodList([1, 2, 3, 4], P). -> P = 8 (unique sol)
% 							    false (bc it tries to find other solutions
% trace, (prodList([1, 2, 3, 4], P)).
    


% Given a list of numbers, remove all increasing sequences
% [1, 2, 4, 6, 5, 7, 8, 2, 1] => [2, 1]
% removeIncreasingSequences(e1, e2...) = { removeIncreasingSequences(e1, e2...), if l1 < l2 >= l3, n >= 3
%   								       removeIncreasingSequences(e2, e3...), if l1 < l2, l3, n >= 3
%										   l1 U removeIncreasingSequences(e3, e4...) if e1 > e2 
%										   [], e1 < e2
%										   e1, if n = 1




