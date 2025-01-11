% A player wants to choose the predictions for N games.
% The predictions can be 1, X, 2.
% Generate all possible bets, considering maximum 2 X's and no 2's on the last position.

generate_predictions(N, AllPredictions) :-
    findall(Prediction,
            (predictions(N, Prediction),
             valid_prediction(Prediction)),
            AllPredictions).

valid_prediction(Prediction) :-
    count_occurrences(Prediction, 'X', XCount),
    XCount =< 2,
    (Prediction = [] ; 
     last(Prediction, Last),
     Last \= '2').

% Count occurrences of an element in a list
count_occurrences([], _, 0).
count_occurrences([E|T], E, Count) :-
    !,
    count_occurrences(T, E, TailCount),
    Count is TailCount + 1.
count_occurrences([H|T], E, Count) :-
    H \= E,
    count_occurrences(T, E, Count).


predictions(0, []).

predictions(N, ['1'|Rest]) :-
    N > 0,
    N1 is N - 1,
    predictions(N1, Rest).

predictions(N, ['2'|Rest]) :-
    N > 1,
    N1 is N - 1,
    predictions(N1, Rest).

predictions(N, ['X'|Rest]) :-
    N > 0,
    N1 is N - 1,
    predictions(N1, Rest).

% ?- generate_predictions(2, Predictions).
% Predictions = [['1','1'], ['1','X'], ['1','2'], ['X','1'], ['X','X'], ['2','1'], ['2','X']].