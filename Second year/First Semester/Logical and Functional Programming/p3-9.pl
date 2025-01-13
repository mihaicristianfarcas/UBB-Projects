% Generate all permutations of N respecting the properties:
% For every 2 <= i <= N there exists 'j' s.t. 1 <= j <= i and |v[i] - v[j]| = 1

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

% checkSol(l1, l2, ..., ln) = true, |l1 - l2| = 1
%                           = checkSol(l1, l3, ..., ln) and checkSol(l2, l3, ..., ln), oth.

checkSol([H1,H2|_]):-
    D is abs(H1 - H2),
    D =:= 1.

checkSol([H1,H2|T]):-
    checkSol([H1|T]),
    checkSol([H2|T]).

findsol(S):-
    findall(Sol, (permutations([1,10,20,30], Sol), checkSol(Sol)), S).

