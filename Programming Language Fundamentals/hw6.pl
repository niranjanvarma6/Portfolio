/* This code written by Chance Giguiere, Logan Helstad, Niranjan Varma, Jackson Williams and Sree Gajula */

/* Exercise 1 */
redefine_system_predicate(when).
when(275,10).
when(261,12).
when(381,11).
when(398,12).
when(399,12).
where(275,owen102).
where(261,dear118).
where(381,cov216).
where(398,dear118).
where(399,cov216).
enroll(mary,275).
enroll(john,275).
enroll(mary,261).
enroll(john,381).
enroll(jim,399).

schedule(Person, Place, Time) :- enroll(Person, Num), where(Num, Place), when(Num, Time).

usage(Place, Time) :- where(X, Place), when(X, Time).

conflict(Class1, Class2) :- where(Class1, X), where(Class2, X), Class1\=Class2, when(Class1, Y), when(Class2, Y).

meet(Person1, Person2) :- (enroll(Person1, X), enroll(Person2, Y), where(X,Z), where(Y,Z), Person1\=Person2),((when(X, T), when(Y, T));(when(X, O), when(Y, L), O=:=L-1)).

/* Exercise 2 */

rmdup([], []).
rmdup([L|D], M) :- member(L,D), rmdup(D,M) .
rmdup([L|D], [L|M]) :- \+ member(L,D), rmdup(D,M) .

flat([], []) :- !.
flat([L|S], F) :-
    !,
    flat(L, Y),
    flat(S, Z),
    append(Y, Z, F).
flat(L, [L]).

