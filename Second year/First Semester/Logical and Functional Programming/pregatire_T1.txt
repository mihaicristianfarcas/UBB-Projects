% Remove all occurrences of a maximum value from a list of integer numbers.
% maxim(l1, l2, ..., ln) = l1, n = 1
%						 = l1, maxim(l2, l3, ..., ln) < l1
%						 = maxim(l2, l3, ..., ln), maxim(l2, l3, ..., ln) > l1

maxim([X], X).

maxim([H|T], X):-
    maxim(T, X1),
    X1 < H,
    X is H.
    
maxim([H|T], X):-
    maxim(T, X1),
    X1 > H,
    X is X1.

% remMax(l1, l2, ..., ln, c1, ..., cm) = [], n = 0
% 						  = l1 U remMax(l2, l3, ..., ln), l1 \= maxim(c1, ..., cm)
% 						  = remMax(l2, l3, ..., ln), l1 =:= maxim(c1, ..., cm)
remMax([], _, []).

remMax([H|T], C, R):-
    maxim(C, X),
    X =:= H,
    remMax(T, C, R).

remMax([H|T], C, [H|R]):-
    remMax(T, C, R).



% Define a predicate to produce a list of pairs (atom n) from an initial list of atoms. In this initial list atom has n occurrences.


numberatom([], []).

numberatom([H|T], [[H,C]|R]) :-
    count_occurrences([H|T], H, C, Remaining),
    numberatom(Remaining, R).

count_occurrences([], _, 0, []).

count_occurrences([H|T], H, C, Remaining) :-
    count_occurrences(T, H, RC, Remaining),
    C is RC + 1.

count_occurrences([X|T], H, C, [X|Remaining]) :-
    X \= H,
    count_occurrences(T, H, C, Remaining).


% Predicate to determine the greatest common divisor of all numbers from a list.

gcd(A, 0, A).

gcd(A, B, GCD) :-
    B \= 0,
    R is A mod B,
    gcd(B, R, GCD).


gcdList([X], X).

gcdList([H1,H2|T], GCD) :-
    gcd(H1, H2, TempGCD),
    gcdList([TempGCD|T], GCD).



% Predicate to transform a list in a set, considering the first occurrences.
% e.g. [1, 2, 3, 1, 2] => [1, 2, 3]

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

nrOcc([_|T], N, C):-
    nrOcc(T, N, C).

% transform(l1, l2, ..., ln) = [], n = 0
%							 = l1 U transform(l2, l3, ..., ln)

% transform(L - list, R - list)
% flow model (i, o)

???





% Predicate to add a value v after 1-st, 2-nd, 4-th, 8-th, … element in a list.
% addValue(l1, l2, ..., ln, I, C, V) = [], n = 0
%								  = l1 U addValue(l2, l3, ..., ln, I, C, V), C mod I \= 0
%								  = l1 U V U addValue(l2, l3, ..., ln, I, C, V), C mod I == 0
% addValue(L - list, I - index, C - counter, V - value, R - list)
% flow model (i, i, i, i, o)

addValue([], _, _, _, []).

addValue([H|T], I, C, V, [H|R]):-
    \+ I mod  C=:= 0,
    C1 is C + 1,
    addValue(T, I, C1, V, R).

addValue([H|T], I, C, V, [H,V|R]):-
    I mod C =:= 0,
    I1 is I * 2,
    C1 is C + 1,
    addValue(T, I1, C1, V, R).



% Given a linear numerical list write a predicate to remove all sequences of consecutive values. 
% e.g. remove([1, 2, 4, 6, 7, 8, 10], L) will produce L=[4, 10].
% 
% remove(l1, l2, ..., ln) = [], n = 0
% 						  = l1, n = 1
% 						  = l1 U remove(l2, l3, ..., ln), l1 + 1 < l2
% 						  = remove(skip_consecutive(l1, l2, l3, ..., ln), l1 + 1 = l2
% remove(L - list, R - list)
% flow model (i, o)
%
% skip_consecutive...

remove([], []).

remove([X], [X]).

remove([X, Y | Rest], Result) :-
    Y =:= X + 1, 
    skip_consecutive(Y, Rest, NewRest),
    remove(NewRest, Result).

remove([X, Y | Rest], [X | Result]) :-
    Y =\= X + 1,
    remove([Y | Rest], Result).

skip_consecutive(_, [], []).

skip_consecutive(Prev, [Next | Rest], NewRest) :-
    Next =:= Prev + 1,
    skip_consecutive(Next, Rest, NewRest).

skip_consecutive(_, List, List).


% b. For a heterogeneous list, formed from integer numbers and list of numbers; write a predicate to delete from every sublist all sequences of consecutive values.
% e.g. [1, [2, 3, 5], 9, [1, 2, 4, 3, 4, 5, 7, 9], 11, [5, 8, 2], 7] => [1, [5], 9, [4, 7, 9], 11, [5, 8, 2], 7] 

% math model...

remSeq([], []).

remSeq([H|T], [HR|R]):-
    is_list(H),
    remove(H, HR),
    remSeq(T, R).

remSeq([H|T], [H|R]):-
    \+ is_list(H),
    remSeq(T, R).
    


% Predicate to determine the proper divisors of a number.
% divisors(E, C) = [], E is prime
% 			 	 = C U divisors(E, C+1), E mod C == 0
% divisors(E - number, C - number, R - list)
% flow model (i, i, o)

divisors(N, C, []):-
    C >= N,
    !.

divisors(E, C, [C|R]):-
    E mod C =:= 0,
    C1 is C + 1,
    divisors(E, C1, R).

divisors(E, C, R):-
    \+ E mod C =:= 0,
    C1 is C + 1,
    divisors(E, C1, R).

% For a heterogeneous list, formed from integer numbers and list of numbers, define a predicate to add in every sublist the divisors of every element.
% e.g. [1, [2, 5, 7], 4, 5, [1, 4], 3, 2, [6, 2, 1], 4, [7, 2, 8, 1], 2] => [1, [2, 5, 7], 4, 5, [1, 4, 2], 3, 2, [6, 2, 3, 2, 1], 4, [7, 2, 8, 2, 4, 1], 2]

mainDivisors(N, R):-
    divisors(N, 2, R).

concatenate([], L, L).
concatenate([H|T], L, [H|R]):-
	concatenate(T, L, R).


addDivs([], []).
addDivs([H|T], [H|R] ):-
    mainDivisors(H, Divs),
    concatenate(Divs, TempRes, R),
    addDivs(T, TempRes).

addEveryList([], []).
addEveryList([H | T], [R1 | R]):-
    is_list(H),
    !,
    addDivs(H, R1),
    addEveryList(T, R).
addEveryList([H | T], [H | R]):-
    addEveryList(T, R).


% Predicate to turn a list into a set keeping the last appearances.
list_to_set([], []).

list_to_set([H|T], Set) :-
    member(H, T),
    list_to_set(T, Set).
list_to_set([H|T], [H|Set]) :-
    \+ member(H, T),
    list_to_set(T, Set).


