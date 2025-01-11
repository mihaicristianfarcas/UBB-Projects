% Generate all strings of N pairs of correctly balanced parentheses

parentheses(N, AllSolutions) :-
    findall(String,
            % convert sequence to string after generating it
            (sequence(N, 0, 0, Sequence),
             atomic_list_concat(Sequence, '', String)),
            AllSolutions).

sequence(0, Open, Close, []) :-
    Open =:= Close.

sequence(N, Open, Close, [')'|Seq]) :-
    N > 0,
    Open > Close,
    Close1 is Close + 1,
    N1 is N - 1,
    sequence(N1, Open, Close1, Seq).

sequence(N, Open, Close, ['('|Seq]) :-
    N > 0,
    Open < N,
    Open1 is Open + 1,
    N1 is N - 1,
    sequence(N1, Open1, Close, Seq).

% ?- parentheses(6, S).
% S = ['()(())', '(()())', '((()))'].