hombre(hans).
hombre(erik).
hombre(david).
hombre(samuel).
hombre(andres).
hombre(herman).
hombre(alejandro).
hombre(daniel).

mujer(regina).
mujer(paola).
mujer(angela).
mujer(helga).
mujer(sharon).

es_hijo_de(erik, hans).
es_hijo_de(david, hans).
es_hijo_de(samuel, hans).
es_hijo_de(erik, regina).
es_hijo_de(david, regina).
es_hijo_de(samuel, regina).
es_hijo_de(hans, herman).
es_hijo_de(alejandro, angela).
es_hijo_de(daniel, sharon).

es_hija_de(paola, regina).
es_hija_de(paola, andres).
es_hija_de(regina, angela).
es_hija_de(helga, herman).
es_hija_de(sharon, herman).

es_padre_de(P, H):-
    hombre(P), es_hijo_de(H, P);es_hija_de(H, P).

es_madre_de(M, H):-
    mujer(M), es_hijo_de(H, M);es_hija_de(H, M).

es_hermano_de(H, X):-
    hombre(H),
    (
        (es_padre_de(P, H), es_padre_de(P, X));
        (es_madre_de(M, H), es_madre_de(M, X))
    ),
    H \= X.

es_hermana_de(H, X):-
    mujer(H),
    (
        (es_padre_de(P, H), es_padre_de(P, X));
        (es_madre_de(M, H), es_madre_de(M, X))
    ),
    H \= X.

es_abuelo_de(A, N):-
    hombre(A),
    (
    (es_hijo_de(P, A),es_hijo_de(N, P));
    (es_hijo_de(P, A),es_hija_de(N, P));
    (es_hija_de(M, A),es_hijo_de(N, M));
    (es_hija_de(M, A),es_hija_de(N, M)) 
    ).
    
es_abuela_de(A, N):-
    mujer(A),
    (
    (es_hijo_de(P, A),es_hijo_de(N, P));
    (es_hijo_de(P, A),es_hija_de(N, P));
    (es_hija_de(M, A),es_hijo_de(N, M));
    (es_hija_de(M, A),es_hija_de(N, M))
    ).

es_nieto_de(N, A):-
    (hombre(N),
    (es_abuelo_de(A, N);es_abuela_de(A, N))). 

es_nieta_de(N, A):-
    (mujer(N),
    (es_abuelo_de(A, N);es_abuela_de(A, N))).

es_ancestro_de(A, D):-
    (es_hijo_de(D, A);es_hija_de(D, A)).

es_ancestro_de(A, D):-
    ((es_hijo_de(D, P);es_hija_de(D, P)),
    es_ancestro_de(A, P)).

es_tio_de(T, S):-
	(hombre(T),
		(
  			(   
        		es_hijo_de(S, P) , es_hermano_de(T, P)
        	);
    		(   
        		es_hija_de(S, P) , es_hermano_de(T, P)
        	)
		)
	).

es_tia_de(T, S):-
	(   
		mujer(T),
		(
  			(   
        		es_hijo_de(S, P) , es_hermana_de(T, P)
        	);
    		(   
        		es_hija_de(S, P) , es_hermana_de(T, P)
        	)
		)
	).

es_sobrino_de(S, T):-
    hombre(S),(
    	es_tia_de(T,S);es_tio_de(T,S)
    ).

es_sobrina_de(S, T):-
    mujer(S),(
    	es_tia_de(T,S);es_tio_de(T,S)
    ).


