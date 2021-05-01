#include <iostream>
#include <array>
#include <algorithm>
#include <memory>
#include <map>

#include "a_star.h"

/* Ejemplo de solucion al problema Cruzando el rio utilizando A* */

using namespace ia;

constexpr int T_ESTADO = 4;
using Estado = std::array<bool, T_ESTADO>;
std::map<Estado, Nodo<Estado>*> ESTADOS_CONOCIDOS;

const std::array<Estado, 6> INVALIDOS{
    Estado{0,1,1,1}, Estado{0,1,1,0}, Estado{0,0,1,1},
    Estado{1,0,0,0}, Estado{1,0,0,1}, Estado{1,1,0,0}
};

bool estadoValido(Estado estado){
    for(int i = 0; i < 6; i++){
        if (estado == INVALIDOS[i]){
            return false;
        }
    }
    return true;
}


class NodoRio : public Nodo<Estado> {
private:
    Nodo<Estado>* analizaMovimiento(int pos) const {
        Estado estado = _estado;
        if ((_estado[0] && _estado[pos]) || (!_estado[0] && !_estado[pos])){
            estado[pos] = !estado[pos];
        }
        estado[0] = !estado[0];

        if (estadoValido(estado)){
            auto nodo_vecino = ESTADOS_CONOCIDOS.find(estado);
            if (nodo_vecino == ESTADOS_CONOCIDOS.end())
            {
                auto resultado = ESTADOS_CONOCIDOS.insert({ estado, new NodoRio(estado) });
                nodo_vecino = resultado.first;
            }
            return nodo_vecino->second;
        }

        return NULL;
    }

public:
    NodoRio(Estado estado) : Nodo<Estado>(estado) {}

    ListaAdyacencia<Estado> obtener_vecinos() const {
        ListaAdyacencia<Estado> vecinos;
 
        std::vector<Nodo*> posibles;
        
        for(int i = 0; i < T_ESTADO; i++){
            posibles.push_back(analizaMovimiento(i));
        }

        for(int i = 0; i < T_ESTADO; i++){
            if(posibles[i]){
                vecinos.push_back({1, posibles[i]});
            }
        }

        return vecinos;
    }

     friend std::ostream& operator<<(std::ostream& out_stream, const NodoRio& nodo)
    {
        auto estado = nodo.obtener_estado();
        out_stream << "Este: ";
        for (int i = 0; i < T_ESTADO; ++i)
        {
            out_stream << estado[i] << ' ';
        }
        out_stream << " Oeste: ";
        for (int i = 0; i < T_ESTADO; ++i)
        {
            out_stream << !estado[i] << ' ';
        }
        out_stream << '\n';
        return out_stream;
    }
};

Heuristica<Estado> heuristica = [](Nodo<Estado> *fuente, Nodo<Estado> *destino) {
    return 0;
};

int main(){
    NodoRio* fuente = new NodoRio(Estado{1,1,1,1});
    NodoRio* destino = new NodoRio(Estado{0,0,0,0});

    ESTADOS_CONOCIDOS.insert({fuente->obtener_estado(), fuente});
    ESTADOS_CONOCIDOS.insert({destino->obtener_estado(), destino});

    auto camino = resolver_a_star(fuente, destino, heuristica);

        for (const auto& nodo : camino)
    {
        std::cout << *dynamic_cast<NodoRio*>(nodo) << '\n';
    }

    for(const auto& pair: ESTADOS_CONOCIDOS){
        delete pair.second;
    }
}