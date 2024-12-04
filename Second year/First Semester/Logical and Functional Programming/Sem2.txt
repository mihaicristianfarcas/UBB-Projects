% suma(l1, l2, ..., ln) = 0, n = 0
%					    = l1 + suma(l2,..., ln), n > 0
% suma(L - list, R - nummber) 
% flow model (i, o) (i, i)
suma([], 0).
suma([H|T], R):-
    suma(T, TR),
    R is H + TR.

% suma(l1, l2, ..., ln, C) = C, n = 0
%					       = sumc(l2,..., ln, C + l1), n > 0
% sumc(L - list, C - number, R - number)
% flow model (i, i, o)
sumc([], C, C).
sumc([H|T], C, S):-
    NewC is C + H,
    sumc(T, NewC, S).


mainSumC(L, S):- sumc(L, 0, S).



% nrOcc(l1, l2, ..., ln, N) = 0, n = 0
%							= 1 + nrOcc(l2, l3, ..., ln), l1 = N
%							= nrOcc(l2, l3, ..., ln), l1 != N
% nrOcc(l - list, N - value, R - number of occ.)
% flow model (i, i, o)
nrOcc([], _, 0).
nrOcc([E|T], E, R):- 
    nrOcc(T, E, R1),
    R is R1 + 1.
    
% not false and only one result => deterministic predicate
    
nrOcc([H|T], E, R):-
    H =\= E,
    nrOcc(T, E, R).
    
% nOcc(l1, l2, ..., ln, E, C) = C, n = 0
%							  = nOcc(l2, ..., ln, E, C + 1), l1 = E
% 							  = nOcc(l2, ..., ln, E, C), l1 != E
% nOcc(L - list, E - number, C - number, R - number)
% flow model (i, i, i, i) (i, i, i, o)
nOcc([], _, C, C).

nOcc([H|T], E, C, R):-
    H =:= E,
    !, % the cut, aka if H = E don't go further looking for other solution
    Cn is C + 1,
    nOcc(T, E, Cn, R).

nOcc([H|T], E, C, R):-
    H =\= E,
    nOcc(T, E, C, R).


mainNOC(L, E, R):- nrOcc(L, E, R).



% Remove from a list the elements that appear only once.
%


% Compute the gcd of all elements in a list.
% gcd(a, b) = b, a mod b = 0
%			= gcd(b, a mod b), a mod b != 0
% gcdList(l1, l2, ..., ln) = l1, n = 1
%						   gcdList(gcd(l1, l2), l3, ..., ln), n > 1
%					   OR  





    
    
    
    
    
    
    
    
    
    
    
    
    
    
