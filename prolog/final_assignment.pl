% male facts
% male(X) means X is a male
male(daniel).
male(michael).
male(matthew).
male(john).
male(paul).
male(todd).
male(mark).


% female facts
% female(X) means X is a female
female(amy).
female(elizabeth).
female(tina).
female(anu).
female(carey).

% parent facts
% parents(X, Y) means X is the parent of Y
parent(todd,daniel).
parent(todd,elizabeth).
parent(todd,matthew).
parent(todd,michael).
parent(amy,daniel).
parent(amy,elizabeth).
parent(amy,matthew).
parent(amy,michael).
parent(john,todd).
parent(anu,todd).
parent(paul,amy).
parent(tina,amy).
parent(tina,mark).
parent(paul,mark).
parent(anu,carey).
parent(john,carey).

father(X, Y) :- male(X), parent(X, Y).
mother(X, Y) :- female(X), parent(X, Y).
child(X, Y) :- parent(Y, X).
son(X, Y) :- child(X, Y), male(X).
daughter(X, Y) :- child(X, Y), female(X).
grandparent(X, Y) :- parent(X, Z), parent(Z, Y).
grandfather(X, Y) :- grandparent(X, Y), male(X).
grandmother(X, Y) :- grandparent(X, Y), female(X).

% aunt(X,Y) means X is the Aunt of Y
aunt(X, Y) :- parent(Z, Y), fullsibling(X, Z), female(X).
aunt(X, Y) :- parent(Z, Y), halfsibling(X, Z), female(X).

% uncle(X,Y) means X is the Uncle of Y
uncle(X, Y) :- parent(Z, Y), fullsibling(X, Z), male(X).
uncle(X, Y) :- parent(Z, Y), halfsibling(X, Z), male(X).

fullsibling(X, Y) :- mother(M, X), mother(M, Y), father(F, X), father(F, Y), X\=Y. 
halfsibling(X, Y) :- mother(M, X), mother(M, Y), father(F, X), father(G, Y), F\=G, X\=Y.
halfsibling(X, Y) :- mother(M, X), mother(N, Y), father(F, X), father(F, Y), M\=N, X\=Y.


%reverse(R, L) mean R is the reversed version of L
reverse([], []). % special case where the list is empty will never be used during our recursion on an empty list
reverse([H],[H]) :- !. % base case for a one item list is this situation where we have a one item list
reverse(R, [H|T]) :- reverse(TR, T), append(TR, [H], R).




