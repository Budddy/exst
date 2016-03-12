node(1..3).
edge(1,2).
edge(1,3).
edge(2,3).
col(r).
col(g).
col(b).

{color(X,C):col(C)} == 1:-node(X).
:-edge(X,Y),color(X,C),color(Y,C).
