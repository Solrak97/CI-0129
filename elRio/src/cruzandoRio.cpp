#include <iostream>
#include <array>
#include <algorithm>
#include <memory>
#include <map>
#include <vector>

#include "a_star.h"

/* Ejemplo de solucion al problema Cruzando el rio utilizando A* */

using namespace ia;

const int BIT_SPACESIZE = 4;
using Estado = std::array<bool, BIT_SPACESIZE>;

std::map<Estado, Nodo<Estado>*> EstadosVisitados; //Tengo que cambiar esto para no loopear



//Estas funciones van a ser utiles para representar el estado complemento
int estadoToInt(Estado estado){
    return (estado[0] * 8 + estado[1] * 4 + estado[2] * 2 + estado[3]);
}

bool validadorDeEstado(Estado estado){
    int val = estadoToInt(estado);
    return (val != 3 && val != 6 && val != 9 && val != 12);
}

void bitFlip(std::array<bool, BIT_SPACESIZE>& bitSpace){
    for(int i = 0; i < BIT_SPACESIZE; i++){
        bitSpace[i] = !bitSpace[i]; 
    }
}

void arrayPrint(std::array<bool, BIT_SPACESIZE> bitSpace){
    for(const auto& s: bitSpace)
        std::cout << s << ' ';
    std::cout << '\n';
}





class NodoPuzzleRio : public Nodo<Estado>{
public:
    ListaAdyacencia<Estado> obtener_vecinos() const override {
        ListaAdyacencia<Estado> vecinos;

        vecinos = analizar_posibles_movimientos();
    }

private:
    ListaAdyacencia<Estado> analizar_posibles_movimientos() const {
        //_estado Falta sacar los estados posibles y verificarlos con la funcion de arriba
    }

    double 

};

int main(){
    Estado estadoInicial {0,0,0,0};
    Estado estadoFinal {1,1,1,1};


    std::cout << validadorDeEstado(estadoInicial);
    std::cout << validadorDeEstado(estadoFinal);

}