% Det. all possibilities of coloring an n-countries map with m colors, s.t. no adjacent countries have the same color.
% insertElement(l1, l2, ..., ln, e) = e U l1 U l2 ... U ln, n >= 0
%                                   = l1 U insertElement(e, l2, ..., ln), n > 0
% (i, i, o)

insertElement(L, E, [E|L]).
insertElement([H|T], E, [H|Res]):-
    insertElement(T, E, Res).

% findsol(S):- findall(Sol, insertElement([1,2,3,4,5], 6, Sol), S).

% permutations(l1, l2, ..., ln) = [], n = 0
%                               = insertElement(l1 U permutations(l2, l3, ..., ln)), oth.

permutations([], []).
permutations([H|T], P):-
    permutations(T, Res),
    insertElement(Res, H, P).

% checkSol(l1, l2, ..., ln) = true, n = 1
%                           = checkSol(l2, l3, ..., ln), l1 != l2
checkSol([_]).
checkSol([H1,H2|T]):-
    H1 =\= H2,
    checkSol([H2|T]).

findsol(S):-
    findall(Sol, (permutations([1,2,1,3,5,2,4,1], Sol), checkSol(Sol)), S).

