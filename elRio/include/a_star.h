#pragma once
#include <chrono>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>
#include <utility>
#include <memory>
#include <functional>
#include <limits>
#include <cmath>
#include <queue>
#include <map>

namespace ia
{
    /* Constantes */

    // Representa un costo infinito, que es el valor máximo de un double.
    const double INF = std::numeric_limits<double>::max();

    /* Forward declarations */

    // Cada estado del problema se guarda dentro de un Nodo, representado por el tipo T.
    template <typename T>
    class Nodo;

    /* Typedefs */

    // Representa el costo para llegar al un nodo (arista).
    template <typename T>
    using CostoNodo = std::pair<double, Nodo<T> *>;

    // Representa las aristas hacia los vecinos con sus costos.
    template <typename T>
    using ListaAdyacencia = std::vector<CostoNodo<T>>;

    // Es una función que estima el costo de ir de nodo_fuente a nodo_destino utilizando una heurística.
    template <typename T>
    using Heuristica = std::function<double(
        Nodo<T> *nodo_fuente,
        Nodo<T> *nodo_destino)>;

    // Representa una solución encontrada por el algoritmo.
    template <typename T>
    using Camino = std::vector<Nodo<T> *>;

    /* Clases */

    // Cada estado del problema se guarda dentro de un Nodo, representado por el tipo T.
    // Esta plantilla se usa para generar clases abstractas genéricas.
    // Para resolver el problema, DEBE heredar de esta clase e implementar sus métodos virtuales puros.
    template <typename T>
    class Nodo
    {
    protected:
        // miembros propios
        T _estado;

    public:
        // constructores
        Nodo() = default;
        Nodo(T estado) : _estado(estado) {}

        // getters
        const T &obtener_estado() const
        {
            return _estado;
        }

        // Representa la lista de vecinos. DEBE implementar este método al heredar de la clase.
        virtual ListaAdyacencia<T> obtener_vecinos() const = 0;
    };

    // Esta plantilla representa la información que guarda el algoritmo A* para cada nodo.
    template <typename T>
    class EstadoNodo
    {
    public:
        bool visitado;
        double costo_real;
        double costo_estimado;
        Nodo<T> *ancestro;

        EstadoNodo()
            : visitado(false),
              costo_real(INF),
              costo_estimado(INF),
              ancestro(nullptr)
        {
        }

        // Método de fábrica para consturir el estado del nodo fuente fácilmente.
        static EstadoNodo nodo_inicial(double costo_estimado)
        {
            EstadoNodo<T> estado_inicial = EstadoNodo<T>();
            estado_inicial.costo_real = 0;
            estado_inicial.costo_estimado = costo_estimado;

            return estado_inicial;
        }
    };

    // Representa un registro histórico de los nodos por los que ha pasado el algoritmo A*.
    // Se actualiza durante la exploración.
    template <typename T>
    class MemoriaAStar
    {
    public:
        // El estado conocido hasta ahora de cada nodo.
        std::map<Nodo<T> *, EstadoNodo<T>> estado_nodos;
        // Cola de prioridad que ordena los nodos con base en el costo estimado de menor a mayor.
        std::priority_queue<CostoNodo<T>> nodos_por_probar;
    };

    /* Funciones */

    // Implementación del algoritmo A*.
    template <typename T>
    Camino<T> resolver_a_star(Nodo<T> *nodo_fuente, Nodo<T> *nodo_destino, const Heuristica<T> &heuristica)
    {
        assert(nodo_fuente != nullptr);
        assert(nodo_destino != nullptr);

        auto tiempo_inicio = std::chrono::high_resolution_clock::now();

        int nodos_revisados = 0;

        // Inicializar estado del algoritmo.
        MemoriaAStar<T> estado_a_star = MemoriaAStar<T>();

        double costo_estimado_inicial = heuristica(nodo_fuente, nodo_destino);
        EstadoNodo<T> estado_nodo_inicial = EstadoNodo<T>::nodo_inicial(costo_estimado_inicial);

        estado_a_star.estado_nodos.insert({nodo_fuente, estado_nodo_inicial});
        estado_a_star.nodos_por_probar.push({costo_estimado_inicial, nodo_fuente});

        // Se continúa el algoritmo mientras que hayan caminos sin probar.
        while (!estado_a_star.nodos_por_probar.empty())
        {
            // Tomo el nodo donde costo_real + costo_estimado sea menor y lo elimino de la lista.
            Nodo<T> *nodo_actual = estado_a_star.nodos_por_probar.top().second;
            estado_a_star.nodos_por_probar.pop();

            // Si llegó al destino, termina la búsqueda.
            if (nodo_actual == nodo_destino)
                break;

            // Obtener el estado actual del nodo.
            EstadoNodo<T> *estado_nodo_actual = &estado_a_star.estado_nodos[nodo_actual];

            // Si ya se visitó, se puede saltar.
            if (estado_nodo_actual->visitado)
                continue;

            nodos_revisados++;

            // Itera por cada uno de los nodos vecinos.
            for (auto &[distancia_a_vecino, nodo_vecino] : nodo_actual->obtener_vecinos())
            {
                // Obtiene el estado del vecino.
                EstadoNodo<T> *estado_nodo_vecino = &estado_a_star.estado_nodos[nodo_vecino];

                if (estado_nodo_vecino->visitado)
                    continue;

                // Revisamos si encontramos un camino más corto a ese vecino a través del nodo actual.
                if (estado_nodo_actual->costo_real + distancia_a_vecino < estado_nodo_vecino->costo_real || estado_nodo_vecino->costo_real == INF)
                {
                    // Encontramos una mejor ruta, entonces actualizamos el estado del vecino en la Memoria del algoritmo
                    estado_nodo_vecino->ancestro = nodo_actual;
                    estado_nodo_vecino->costo_real = estado_nodo_actual->costo_real + distancia_a_vecino;
                    // Esta es la clave del algoritmo A*, se utiliza el costo real para llegar a este vecino
                    // sumándole el costo estimado del vecino al destino, utilizando la heurística.
                    estado_nodo_vecino->costo_estimado =
                        estado_nodo_vecino->costo_real + heuristica(nodo_vecino, nodo_destino);
                }

                // Descubrimos un nodo nuevo, entonces lo agregamos a la lista de nodos por probar.
                // La lista ordena ascendentemente basándose en el costo real para llegar a ese nodo
                // y le suma el costo estimado dado por la heurística para llegar al destino.
                // Si la heurística es buena (nunca sobreestima, siempre estima un valor menor o igual al costo óptimo real),
                // es garantizado que el camino va a ser el más corto.
                // Usamos el costo estimado negativo para ordenar ascendentemente
                estado_a_star.nodos_por_probar.push({-estado_nodo_vecino->costo_estimado, nodo_vecino});
            }

            // Se marca el nodo como visitado.
            // En este punto se encontró el camino más corto para llegar a este nodo, y ya no hay que actualizarlo más.
            estado_nodo_actual->visitado = true;
        } // Al termiar este while concluye la búsqueda.

        std::cout << "Nodos revisados: " << nodos_revisados << '\n';

        // A continuación se reconstruye el camino (si es que se encontró uno).
        Camino<T> camino;
        // Empezamos desde el nodo destion a devolvernos hasta la fuente
        EstadoNodo<T> *estado_nodo_destino = &estado_a_star.estado_nodos[nodo_destino];

        // Si hay un ancestro para el nodo destino, se encontró el camino más corto.
        if (estado_nodo_destino->ancestro != nullptr)
        {
            camino.push_back(nodo_destino);
            Nodo<T> *nodo_en_camino = estado_nodo_destino->ancestro;
            EstadoNodo<T> *estado_nodo_en_camino = &estado_a_star.estado_nodos[nodo_en_camino];

            // Nos devolvemos por el camino desde el destino hasta el nodo fuente para reconstruirlo.
            while (nodo_en_camino != nodo_fuente)
            {
                camino.push_back(nodo_en_camino);
                nodo_en_camino = estado_nodo_en_camino->ancestro;
                estado_nodo_en_camino = &estado_a_star.estado_nodos[nodo_en_camino];
            }

            camino.push_back(nodo_fuente);
            // Retornamos el camino desde el nodo fuente al nodo destino, entonces se invierte la lista.
            std::reverse(camino.begin(), camino.end());

            std::cout << "Costo total del camino encontrado: " <<
                estado_a_star.estado_nodos[nodo_destino].costo_real << '\n';
        }
        else
        {
            std::cout << "No se encontró un camino.\n";
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - tiempo_inicio;
        std::cout << "Tiempo de procesamiento: " << duration.count() << 's';
        std::cout << std::endl;

        return camino;
    }

    /* heurísticas */

    // Cuando se usa esta función como la huerística hace que A* se comporte como el algoritmo de Dijkstra.
    template <typename T>
    Heuristica<T> heuristica_dijkstra = [](Nodo<T> *nodo_fuente, Nodo<T> *nodo_destino) {
        return 0.0;
    };

    // Calcula distancia en una cuadrícula.
    double distancia_manhattan(int x1, int y1, int x2, int y2)
    {
        return (double)abs(x1 - x2) + (double)abs(y1 - y2);
    }

    // Calcula distancia usando el Teorema de Pitágoras
    double distancia_euclideana(double x1, double y1, double x2, double y2)
    {
        return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    }
}
