#include <iostream>
#include <memory>

#include "a_star.h"

/* Ejemplo de solución al problema de encontrar el camino más corto en un grafo. */

using namespace std;
using namespace ia;

// Extiende a la clase abstracta Nodo con una representación clásica de grafos utilizando listas de adyacencia.
template <typename T>
class NodoGrafo : public Nodo<T>
{
public:
    NodoGrafo(T dato) : Nodo<T>(dato) {}

    // Se agrega un vecino a la lista de adyacencia del nodo.
    void agregar_vecino(NodoGrafo<T> *nodo_vecino, double peso, bool es_dirigido = true)
    {
        assert(nodo_vecino != nullptr);

        _vecinos.push_back({peso, nodo_vecino});
        if (!es_dirigido)
            nodo_vecino->agregar_vecino(this, peso);
    }

    // No es necesario calcular la lista de vecinos, porque ya está almacenada en la lista de adyacencia del nodo.
    ListaAdyacencia<T> obtener_vecinos() const override
    {
        return _vecinos;
    }

protected:
    // Lista de adyacencia para el nodo.
    ListaAdyacencia<T> _vecinos = ListaAdyacencia<T>();
};

int main()
{
    // Se utiliza char como argumento de la plantilla para etiquetar los nodos.
    NodoGrafo<char> *nodo_A = new NodoGrafo<char>('A');
    NodoGrafo<char> *nodo_B = new NodoGrafo<char>('B');
    NodoGrafo<char> *nodo_C = new NodoGrafo<char>('C');
    NodoGrafo<char> *nodo_D = new NodoGrafo<char>('D');
    NodoGrafo<char> *nodo_E = new NodoGrafo<char>('E');
    NodoGrafo<char> *nodo_F = new NodoGrafo<char>('F');

    // Se genera un nodo básico.
    nodo_A->agregar_vecino(nodo_B, 2, false);
    nodo_A->agregar_vecino(nodo_E, 1, false);
    nodo_A->agregar_vecino(nodo_F, 3, false);

    nodo_B->agregar_vecino(nodo_C, 1, false);

    nodo_C->agregar_vecino(nodo_D, 2, false);
    nodo_C->agregar_vecino(nodo_E, 1, false);

    nodo_E->agregar_vecino(nodo_D, 5, false);
    nodo_E->agregar_vecino(nodo_F, 1, false);

    nodo_F->agregar_vecino(nodo_D, 1, false);

    // Encontrar el camino más corto del NodoA al NodoD utilizando A*.
    // Aquí también puede probar con diferentes heurísticas.
    auto camino = resolver_a_star(nodo_A, nodo_D, heuristica_dijkstra<char>);

    // Imprimir solución.
    std::cout << "Camino:\n";
    for (auto nodo : camino)
    {
        std::cout << nodo->obtener_estado() << std::endl;
    }

    // Limpieza.
    delete nodo_A;
    delete nodo_B;
    delete nodo_C;
    delete nodo_D;
    delete nodo_E;
    delete nodo_F;
}
