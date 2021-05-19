%Clasificacion de ingredientes
ingrediente_comun(I):-
    member(I,[arroz, frijoles, pasta_sin_huevo, tomate, lechuga,
        chile, maíz, zucchini, hongos, cebolla, sal, repollo,
		orégano, apio, remolacha, papa, aceite, tofu, leche_de_almendra,
        café, chocolate, agua, vinagre, fresas]).

carne(I):-
	member(I,[lomito, pollo, pescado, atun]).

producto_animal(I):-
    member(I,[leche, mantequilla, huevo, miel, queso]).


%Recetas
recetas(R):-
    member(R,[pasta_basica, hongos_al_horno, ensalada_cesar, especiales, helados]).

es_ingrediente_de(pasta, pasta_basica).
es_ingrediente_de(oregano, pasta_basica).
es_ingrediente_de(tomate, pasta_basica).
es_ingrediente_de(aceite, pasta_basica).
es_ingrediente_de(sal, pasta_basica).
    
es_ingrediente_de(hongos, hongos_al_horno).
es_ingrediente_de(queso, hongos_al_horno).
es_ingrediente_de(mantequilla, hongos_al_horno).
es_ingrediente_de(sal, hongos_al_horno).

es_ingrediente_de(lechuga, ensalada_cesar).
es_ingrediente_de(tomate, ensalada_cesar).
es_ingrediente_de(sal, ensalada_cesar).
es_ingrediente_de(vinagre, ensalada_cesar).

es_ingrediente_de(miel, helados).
es_ingrediente_de(leche, helados).
es_ingrediente_de(fresas, helados).

especial(P, G, E):-
