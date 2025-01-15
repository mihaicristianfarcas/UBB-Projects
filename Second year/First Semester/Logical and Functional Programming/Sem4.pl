% subset(l1, l2, ..., ln) = [], n = 0
%						  = l1 U subset(l2, l3, ..., ln), n > 0
%						  = subset(l2, l3, ..., ln), n > 0
% subset(L - list, R - list)
% flow model (i, o)

subset([], []).

subset([H|T], [H|R]):-
    subset(T, R).

subset([_|T], R):-
    subset(T, R).
				  
% combinations(l1, l2, ..., ln, k) = l1, k = 1
% 								   = combinations(l2, l3, ..., ln, k), k >= 1
% 								   = l1 U combinations(l2, l3, ..., ln, k - 1), k > 1
% combinations(L - list, K - integer, R - list)
% flow model (i, i, o)

combinations([H|_], 1, [H]).

combinations([_|T], K, R):-
    K >= 1,
    combinations(T, K, R).

combinations([H|T], K, [H|R]):-
    K > 1,
    K1 is K - 1,
    combinations(T, K1, R).

% insertElement(e, l1, l2, ..., ln) = e U l1, l2, ..., ln, n >= 0
% 									= l1 U insertElement(e, l2, l3, ..., ln), n > 0
% insertElement(E - element, L - list, R - list)
% flow model (i, i, o)

insertElement(E, L, [E|L]).
insertElement(E, [H|T], [H|R]):-
    insertElement(E, T, R).

% permutations(l1, l2, ..., ln) = [], n = 0
% 								= insertElement(l1, permutations(l1, l2, ..., ln)), n > 0
% permutations(L - list, R - list)
% flow model (i, o)

permutations([], []).
permutations([H|T], R):-
    permutations(T, P),
	insertElement(H, P, R).
              
                 
% arrangements(l1, l2, ..., ln, k) = l1, k = 1
% 								   = arrangements(l2, l3, ..., ln k), k >= 1, n > 0
% 								   = insertElement(l1, arrangements(l2, l3, ..., ln, k - 1)), k > 1, n > 0
% arrangements(L - list, K - integer, R - list)
% flow model (i, i, o)

arrangements([H|_], 1, [H]).

arrangements([_|T], K, R):-
    K >= 1,
    arrangements(T, K, R).

arrangements([H|T], K, R):-
    K > 1,
    K1 is K - 1,
    arrangements(T, K1, A),
    insertElement(H, A, R).

% Given a list, generate all arrangements of K elements from the list that have the product P and the sum S.
% e.g. [1, 2, 3, 10], K = 2, P = 30, S = 13 => R = [[3, 10], [10, 3]].

suma([], 0).
suma([H | T], S):-
    suma(T, S1),
    S is S1 + H.

prod([], 1).
prod([H | T], P):-
    prod(T, P1),
    P is P1 * H.


% oneSol(L, K, S, P) = a1, a2, ..., ak, a1, a2, ..., ak = (arr(L, K), suma(a1, a2, ..., ak) == S, prod(a1, a2, ..., ak) = P
% oneSol(L - list, K - number, S - number, P - number, A - list) 
% flow model (i, i, i, i, o).

oneSol(L, K, S, P, A):-
    arr(L, K, A),
    suma(A, S),
    prod(A, P).

mainAllSol(L, K, S, P, R):-
    findall(A, oneSol(L, K, S, P, A), R).


% Given a sequence a1, a2, ..., an composed of distinct integer numbers, display all sequences with a valley aspect.
% e.g. [5, 3, 4, 2, 7, 11, 1, 8, 6] => [5, 4, 3, 11] etc.


