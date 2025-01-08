% P3
% 6. Generate the list of all arrangements of K elements of a given list.
% e.g. [2, 3, 4], K=2 => [[2,3], [3,2], [2,4], [4,2], [3,4], [4,3]] (not necessarily in this order)


% insertElement(e, l1, l2, ..., ln) = e U l1, l2, ..., ln, n >= 0
% 									= l1 U insertElement(l2, l3, ..., ln), n > 0
% insertElement(E - element, L - list, R - list)
% flow model (i, i, o)

insertElement(E, L, [E|L]).
insertElement(E, [H|T], [H|R]):-
    insertElement(E, T, R).

% arrangements(l1, l2, ..., ln, k) = l1, k = 1
% 								   = arrangements(l2, l3, ..., ln), k >= 1, n > 0
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

% findall(R, arrangements([1, 2, 3], 2, R), X).

