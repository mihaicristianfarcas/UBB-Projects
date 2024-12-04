% P2

% 1.
% a. Sort a list with removing the double values. e.g. [4, 2, 6, 2, 3, 4] --> [2, 3, 4, 6]

% nrOcc(l1, l2, ..., ln, N) = 0, n = 0
%							= 1 + nrOcc(l2, l3, ..., ln), l1 = N
%							= nrOcc(l2, l3, ..., ln), l1 != N
% nrOcc(L - list, N - value, R - number of occ.)
% flow model (i, i, o)

nrOcc([], _, 0).
nrOcc([E|T], E, R):- 
    nrOcc(T, E, R1),
    R is R1 + 1.
    
nrOcc([H|T], E, R):-
    H =\= E,
    nrOcc(T, E, R).


% remDuplicates(l1, l2, ..., ln, c1, c2, ..., cm) = [], n = 0
% 							 				  = l1 U remDuplicates(l2, l3, ..., ln), member(l1, c1, c2, ..., cm)
% 							 				  = remDuplicates(l2, l3, ..., ln), nrOcc(c1, c2, ..., cm, l1) = 2
% remDuplicates(L - list, C - copy list, R - result list)
% flow model (i, i, o)

remDuplicates([], []).
remDuplicates([H|T], [H|R]) :-
    \+ member(H, T),
    remDuplicates(T, R).

remDuplicates([H|T], R) :-
    member(H, T),
    remDuplicates(T, R).


% insert(e, l1, l2, ..., ln) = e, n = 0
%                            = e U l1 U l2, l3, ..., ln, e <= l1
%                            = l1 U insert(e, l2, ..., ln), e > l1
% insert(e - element, L - list, R - result list)
% flow model (i, i, o)

insert(E, [], [E]).
insert(E, [H|T], [E,H|T]):- 
    E =< H,
    !. 

insert(E, [H|T], [H|R]):-
    E > H, 
    insert(E, T, R). 


% insertionSort(l1, l2, ..., ln) = [], n = 0
%                                = insert(l1, insertionSort(l2, ..., ln))
% insert(L - list, R - result list)
% flow model (i, o)

insertionSort([], []).
insertionSort([H|T], R) :-
    insertionSort(T, SortedTail),
    insert(H, SortedTail, R).

% sortNoDuplicates(l1, l2, ..., ln) = [], n = 0
%									= insertionSort(remDuplicates(l1, l2, ..., ln, l1, l2, ..., ln))
% sortNoDuplicates(L - list, R - result list)
% flow model (i, o)

sortNoDuplicates(L, R) :-
    remDuplicates(L, NoDuplicates),
    insertionSort(NoDuplicates, R).


mainProg(L, R):- sortNoDuplicates(L, R).
% mainProg([1, 2, 1, 3, 4, 1, 2, 4, 5, 6], R).



% b. For a heterogeneous list, formed from integer numbers and list of numbers, write a predicate to sort every sublist with removing the doubles.
% e.g. [1, 2, [4, 1, 4], 3, 6, [7, 10, 1, 3, 9], 5, [1, 1, 1], 7] => [1, 2, [1, 4], 3, 6, [1, 3, 7, 9, 10], 5, [1], 7]

% sublistSortNoDuplicates(l1, l2, ..., l2) = [], n = 0
%										   = l1 U sublistSortNoDuplicates(l2, l3, ..., ln), l1 is an integer
%										   = sortNoDuplicates(l1) U sublistSortNoDuplicates(l2, l3, ..., ln), l1 is a list
% sublistSortNoDuplicates(L - list, R - result list)
% flow model (i, o)

sublistSortNoDuplicates([], []).
sublistSortNoDuplicates([H|T], [SortedH|RT]) :-
    is_list(H),
    sortNoDuplicates(H, SortedH),
    sublistSortNoDuplicates(T, RT).

sublistSortNoDuplicates([H|T], [H|RT]) :-
    \+ is_list(H),
    sublistSortNoDuplicates(T, RT).


mainProg2(L, R):- sublistSortNoDuplicates(L, R).
% mainProg2([1, 2, [4, 1, 4], 3, 6, [7, 10, 1, 3, 9], 5, [1, 1, 1], 7], R).
