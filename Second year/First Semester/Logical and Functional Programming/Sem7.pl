% Write a program to genetare all the subsets with a property that:
% The difference between 2 any cons. elements is a multiple of 3.
% The subsets must have at least 2 elements.
% e.g. [3, 6, 12, 4, 5, 10, 13] -> [[3, 6, 12], [3, 6], [3, 12], [6, 12], [4, 10, 13], [4, 10], [10, 13]]

% subset(l1, l2, ..., ln) = [], n = 0
%                         = subset(l2, l3, ..., ln), n > 0
%                         = l1 U subset(l2, l3, ..., ln), n > 0

subset([],[]).

subset([_|T], R):-
    subset(T, R).

subset([H|T], [H|R]):-
    subset(T, R).

% check-subset(l1, l2, ..., ln) = true, n = 2 and |l1 - l2| % 3 = 0
%                               = check-subset(l2, l3, ..., ln), n > 2 and |l1 - l2| % 3 = 0
%                               = false, oth.

check_subset([H1, H2]):-
    D is abs(H1 - H2),
    D mod 3 =:= 0.

check_subset([H1, H2|T]):-
    D is abs(H1 - H2),
    D mod 3 =:= 0,
    check_subset([H2|T]).

solution(L, R):- subset(L, R), check_subset(R).

all_solutions(L, R):- findall(S, solution(L, S), R).