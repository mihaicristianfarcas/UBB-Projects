% Base case: an empty list or a single-element list is already sorted.
merge_sort([], []).
merge_sort([X], [X]).

% Recursive case: split the list, sort the sublists, and merge them.
merge_sort(List, Sorted) :-
    split(List, Left, Right),        % Split the list into two halves
    merge_sort(Left, SortedLeft),   % Recursively sort the left half
    merge_sort(Right, SortedRight), % Recursively sort the right half
    merge(SortedLeft, SortedRight, Sorted). % Merge the two sorted halves

% Splits a list into two halves.
split([], [], []).
split([X], [X], []).
split([X, Y | Tail], [X | Left], [Y | Right]) :-
    split(Tail, Left, Right).

% Merges two sorted lists into one sorted list.
merge([], Right, Right).
merge(Left, [], Left).
merge([X | TailLeft], [Y | TailRight], [X | Merged]) :-
    X =< Y,
    merge(TailLeft, [Y | TailRight], Merged).
merge([X | TailLeft], [Y | TailRight], [Y | Merged]) :-
    X > Y,
    merge([X | TailLeft], TailRight, Merged).
