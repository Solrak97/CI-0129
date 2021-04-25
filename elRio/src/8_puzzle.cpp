#include <iostream>
#include <array>
#include <algorithm>
#include <memory>
#include <map>

#include "a_star.h"

/* Ejemplo de solución al problema 8 Puzzle utilizando el algoritmo A*. */

using namespace ia;

// Dimensiones de los tableros
constexpr int NUM_FILAS = 3;
constexpr int NUM_COLUMNAS = 3;

// Repesenta el estado del tablero. Como el tablero es una matriz de char,
// se utiliza el caracter ' ' como el espacio vacío,
// y los números como su representación ASCII (caracteres del '1' al '8 ).
using Tablero = std::array<std::array<char, NUM_COLUMNAS>, NUM_FILAS>;

// Almacena los tableros conocidos para no repetir nodos.
// Hay formas de mejorar la implementación actual para eliminar esta tabla de búsqueda.
std::map<Tablero, Nodo<Tablero>*> TABLEROS_CONOCIDOS;

// Extiende a la clase abstracta Nodo y utiliza Tablero como el tipo para la plantilla.
// Esto quiere decir que cada nodo contiene como estado a un tablero.
// Heredar e implementar estos métodos es lo que tienen que hacer para resolver problemas con este código.
class Nodo8Puzzle : public Nodo<Tablero>
{
public:
    Nodo8Puzzle(Tablero tablero) : Nodo<Tablero>(tablero) {}

    // Se implementa el método abstracto que calcula las posibles transiciones o tableros vecinos
    // a los que se puede llegar a partir del tablero actual.
    ListaAdyacencia<Tablero> obtener_vecinos() const override
    {
        ListaAdyacencia<Tablero> vecinos;

        for (int i = 0; i < NUM_FILAS; ++i)
        {
            for (int j = 0; j < NUM_COLUMNAS; ++j)
            {
                if (_estado[i][j] == ' ')
                {
                    vecinos = analizar_posibles_movimientos(i, j);
                }
            }
        }

        return vecinos;
    }

    // to_string
    friend std::ostream& operator<<(std::ostream& out_stream, const Nodo8Puzzle& nodo)
    {
        auto tablero = nodo.obtener_estado();
        for (int i = 0; i < NUM_FILAS; ++i)
        {
            for (int j = 0; j < NUM_COLUMNAS; ++j)
            {
                out_stream << tablero[i][j] << ' ';
            }
            out_stream << '\n';
        }

        return out_stream;
    }

private:
    // Calcula si una fila y una columna pertenecen a la cuadrícula del tablero.
    bool coordenadas_validas(int fila, int columna) const
    {
        return fila >= 0 && fila < NUM_FILAS && columna >= 0 && columna < NUM_COLUMNAS;
    }

    // Calcula los movimientos posibles en el tablero.
    Nodo<Tablero>* analizar_movimiento(int fila_vacia, int columna_vacia,
                         int fila_movimiento, int columna_movimiento) const
    {
        if (coordenadas_validas(fila_movimiento, columna_movimiento))
        {
            Tablero estado;

            std::copy(_estado.begin(), _estado.end(), estado.begin());

            estado[fila_vacia][columna_vacia] = estado[fila_movimiento][columna_movimiento];
            estado[fila_movimiento][columna_movimiento] = ' ';

            auto nodo_vecino = TABLEROS_CONOCIDOS.find(estado);
            if (nodo_vecino == TABLEROS_CONOCIDOS.end())
            {
                auto resultado = TABLEROS_CONOCIDOS.insert({ estado, new Nodo8Puzzle(estado) });
                nodo_vecino = resultado.first;
            }

            return nodo_vecino->second;
        }

        return nullptr;
    }

    // Analiza los movimientos posibles en las direcciones válidas.
    ListaAdyacencia<Tablero> analizar_posibles_movimientos(int fila, int columna) const
    {
        ListaAdyacencia<Tablero> vecinos;

        auto t1 = analizar_movimiento(fila, columna, fila + 1, columna);
        auto t2 = analizar_movimiento(fila, columna, fila - 1, columna);
        auto t3 = analizar_movimiento(fila, columna, fila, columna + 1);
        auto t4 = analizar_movimiento(fila, columna, fila, columna - 1);

        if (t1 != nullptr)
            vecinos.push_back({1, t1});
        if (t2 != nullptr)
            vecinos.push_back({1, t2});
        if (t3 != nullptr)
            vecinos.push_back({1, t3});
        if (t4 != nullptr)
            vecinos.push_back({1, t4});

        return vecinos;
    }
};

// Calcula el costo de asociado a una celda en el tablero. Como se usa la distancia Manhattan,
// este cálculo representa qué tan alejado está esta ficha de su posición correcta en el tablero,
// o cuántos movimientos tengo que hacer para ponerla en su posición correcta.
double calcular_costo(int valor_numerico, int fila, int columna)
{
    int fila_correcta = (valor_numerico - 1) / NUM_COLUMNAS;
    int columna_correcta = (valor_numerico - 1) % NUM_COLUMNAS;

    return distancia_manhattan(fila, columna, fila_correcta, columna_correcta);
}

// La heurística para un tablero es la suma de los costos de todas sus celdas.
Heuristica<Tablero> heuristica_8puzzle = [](Nodo<Tablero> *fuente, Nodo<Tablero> *destino) {
    auto tablero = fuente->obtener_estado();
    double costo = 0.0;
    for (int i = 0; i < NUM_FILAS; ++i)
    {
        for (int j = 0; j < NUM_COLUMNAS; ++j)
        {
            char valor = tablero[i][j];
            if (valor == ' ')
                continue;

            int valor_numerico = valor - '0';

            costo += calcular_costo(valor_numerico, i, j);
        }
    }

    return costo;
};

int main()
{
    // Definimos un estado del tablero aleatorio como el nodo fuente.
    Nodo8Puzzle* nodo_fuente = new Nodo8Puzzle(
        Tablero {{
            {'3', '2', '1'},
            {'4', '8', '6'},
            {'7', '5', ' '},
        }}
    );

    // Definimos el estado del tablero objetivo como el nodo destino.
    Nodo8Puzzle* nodo_destino = new Nodo8Puzzle(
        Tablero {{
            {'1', '2', '3'},
            {'4', '5', '6'},
            {'7', '8', ' '},
        }}
    );

    // Se actualiza el mapa de Tableros Conocidos
    TABLEROS_CONOCIDOS.insert({ nodo_fuente->obtener_estado(), nodo_fuente });
    TABLEROS_CONOCIDOS.insert({ nodo_destino->obtener_estado(), nodo_destino });

    // Ejecutar el algoritmo A* para el problema 8 Puzzle.
    // Note que aquí puede cambiar la heurística para observar diferentes resultados.
    auto camino = resolver_a_star(nodo_fuente, nodo_destino, heuristica_8puzzle);

    // Imprimir solución.
    std::cout << "Camino:\n";
    for (const auto& nodo : camino)
    {
        std::cout << *dynamic_cast<Nodo8Puzzle*>(nodo) << '\n';
    }

    // Limpieza.
    for (const auto& pair : TABLEROS_CONOCIDOS)
    {
        delete pair.second;
    }
}
