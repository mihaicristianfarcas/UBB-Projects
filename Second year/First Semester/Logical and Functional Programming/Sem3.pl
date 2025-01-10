% [[1, 2], 5, [7, 2, 1], s, a, b]

% H|T
% H = [1, 2]
% T = [5, [7, 2, 1], s, a, b]
% is_list(H)
% atomic(H)
% number(H)

% Given a heterogeneous list composed of numbers, symbols, and lists of numbers, remove the sublists
% [[1, 2], 5, [7, 2, 1], s, a, b] => [5, s, a, b]
% process(l1, l2, ..., ln) = [], n = 0
%						   = process(l2, l3, ..., ln), l1 is a list (l11, l12, ..., l1m)
%						   = l1 U process(l2, l3, ..., ln), l1 is not a list
% process(L - list, R - list)
% flow model (i, o)

process([], []).
process([H|T], R):-
    is_list(H),
    !,
    process(T, R).
process([H|T], [H|R]):-
%   not(is_list(H)) or \+ is_list(H) or atomic(H)
    process(T, R).


% Given a list of numbers and sublists of numbers, substitute each sublists for which the sum of the elements is odd with the first element of that list. Assume sublists have at least one element.
% e.g. [1, 2, [2, 4], 7, 3, [4, 6, 7], [1], 8, 10, [3, 2]] => [1, 2, [2, 4], 7, 3, 4, 1, 8, 10, 3]
suma([], []).
suma([H|T], S):-
    suma(T, TS),
    S is H + TS.

% proc(l1, l2, ..., ln) = [], n = 0
%						= l11 U proc(l2, l3, ..., ln), l1 is a list (l11, l12, ..., l1m) and suma(l1) mod 2 = 1
% 						= l1 U proc(l2, l3, ..., ln), either l1 is a list and suma(l1) mod 2 == 0 or l1 is not a list
% proc(L - list, R - list)
% flow model (i, o)
proc([], []).
proc([H|T], [H1|R]):-
    is_list(H),
    suma(H, S),
    S mod 2 =:= 1,
    !,
    H = [H1|_],
    proc(T, R).
proc([H|T], [H|R]):-
    proc(T, R).

% remOdd(l1, l2, ..., ln) = [], n = 0
%						  = l1 U remOdd(l2, l3, ..., ln), l1 mod 2 = 0
%						  = remOdd(l2, l3, ..., ln), l1 mod 2 != 0

% mountain(l1, l2, ..., ln, F) = true, n = 1, F = 1
%							   = mountain(l2, l3, ..., ln, 0), l1 < l2, F = 0
%							   = mountain(l2, l3, ..., ln, 1), l1 > l2, F = 1
%							   = false, otherwise

% mainMountain(l1, l2, ..., ln)

% processData(l1, l2, ..., ln) = [], n = 0

processData([], []).
processData([H|T], [H1, R]):-
    is_list(H),
    mainMountain(H),
    !,
    remOdd(H, H1),
    processData(T, R).
processData([H|T], [H|R]):-
    processData(T, R).

% ... poza


% Remove increasing sequences of numbers from a list
% [1, 2, 4, 4, 7 ,9, 0, 1, 1] => [1]
% l1 < l2 < l3
% l1 < l2 > l3
% l1 > l2

% remInc(l1, l2, ..., ln) = [], n = 0
%						  = [l1], n = 1
%						  = [], n = 2, l1 < l2
%						  = l1 U remInc(l2, l3, ..., ln), l1 >= l2
%						  = remInc(l2, l3, ..., ln), l1 < l2 < l3
%						  = remInc(l3, l4, ..., ln), l1 < l2 > l3
% remInc

remInc([], []).
remInc([H], [H]).
remInc([H1, H2], []):- H1 > H2.
remInc([H1,H2|T], [H1,R]):-
    H1 >= H2,
    remInc([H2|T], R).
remInc([H1,H2,H3|T], R):-
    H1 < H2,
    H2 < H3,
    remInc([H2,H3|T], R).
remInc([H1,H2,H3|T], R):-
    H1 < H2,
    H2 >= H3,
    remInc([H3|T], R).