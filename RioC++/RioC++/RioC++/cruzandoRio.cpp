#include <iostream>
#include <array>
#include <algorithm>
#include <memory>
#include <map>
#include <vector>

#include "a_star.h"

/* Ejemplo de solucion al problema Cruzando el rio utilizando A* */

using namespace ia;

class Estado {
public:
    std::array<char, 4> Este;
    std::array<char, 4> Oeste;

    Estado(std::array<char, 4> Este, std::array<char, 4> Oeste){
        this -> Este = Este;
        this -> Oeste = Oeste;
    }

    Estado(const Estado& estado){
        this -> Este = estado.Este;
        this -> Oeste = estado.Oeste;
    }

    bool operator == (Estado e){
        return (this -> Este == e.Este && this -> Oeste == e.Oeste);
    }

    
    Estado swap(int i) const {
        Estado copia = Estado(this->Este, this->Oeste);
        char T = copia.Este[i];
        copia.Este[i] = copia.Oeste[i];
        copia.Oeste[i] = T;
        return copia;
    }

    
    bool esValido(){
        //Quiero mover esto para que sea estatico al menos, declararlo para validar es horrible!
        std::vector<Estado> invalidos{
            Estado(std::array<char, 4>{'X','P','C','R'}, std::array<char, 4>{'G','X','X','X'}),
            Estado(std::array<char, 4>{'G','X','X','X'}, std::array<char, 4>{'X','P','C','R'}),
            Estado(std::array<char, 4>{'X','P','C','X'}, std::array<char, 4>{'G','X','X','R'}),
            Estado(std::array<char, 4>{'G','X','X','R'}, std::array<char, 4>{'X','P','C','X'}),
            Estado(std::array<char, 4>{'X','X','C','R'}, std::array<char, 4>{'G','P','X','X'}),
            Estado(std::array<char, 4>{'G','P','X','X'}, std::array<char, 4>{'X','X','C','R'}),        
        };

        for(int i = 0; i < 6; i++){
            if(*this == invalidos[i]){
                return false;
            }
        }
        return true;
    } 
};


void print(Estado estado){
    std::cout << "Este:  "; 
    for(int i = 0; i < 4; i++){
        std::cout << estado.Este[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Oeste: ";
    for(int i = 0; i < 4; i++){
        std::cout << estado.Oeste[i] << " ";
    }
    std::cout << std::endl << std::endl;
}


class NodoRio : public Nodo<Estado> {
public:
    NodoRio(Estado estado) : Nodo<Estado>(estado) {}

    Estado terrorista(){
        return _estado;
    }

    ListaAdyacencia<Estado> obtener_vecinos() const override {
        ListaAdyacencia<Estado> vecinos;
        std::vector<NodoRio> listaVecinos = GeneraVecinos();

        for (std::vector<NodoRio>::iterator i = listaVecinos.begin(); i != listaVecinos.end(); ++i){
            vecinos.push_back({1, &*i});
        }


        std::cout << "Print terrorista" << std::endl;


        for (std::vector<NodoRio>::iterator i = listaVecinos.begin(); i != listaVecinos.end(); ++i){
            NodoRio a = *i; 
            print(a.terrorista());
        }
        return vecinos;
    }

private:
    std::vector<NodoRio> GeneraVecinos() const {
        std::vector<NodoRio> vecinos;

        if(_estado.Este[0] == 'G'){         //Si el granjero esta al este
            Estado base = _estado.swap(0);
            
            if(base.esValido()){            
                vecinos.push_back(NodoRio(base));
                print(base);
            }
            
            verificaEste(vecinos, base, 1); //Verifica si el perro esta con el granjero
            verificaEste(vecinos, base, 2); //Verifica si el conejo esta con el granjero
            verificaEste(vecinos, base, 3); //Verifica si el repollo esta con el granjero
        }
        else{
            Estado base = _estado.swap(0);
            
            if(base.esValido()){            
                vecinos.push_back(NodoRio(base));
                print(base);
            }

            verificaOeste(vecinos, base, 1); //Verifica si el perro esta con el granjero
            verificaOeste(vecinos, base, 2); //Verifica si el conejo esta con el granjero
            verificaOeste(vecinos, base, 3); //Verifica si el repollo esta con el granjero
        }
        return vecinos;

    }

//Verifica si una entidad x esta con el granjero
//Luego verifica si el estado es valido
//Finalmente inserta el estado en caso de ser valido
    void verificaEste(std::vector<NodoRio>& vecinos, Estado base, int pos) const {
        if(base.Este[pos] != 'X'){        
                Estado temp(base);
                temp = temp.swap(pos);
                if(temp.esValido()){
                    vecinos.push_back(NodoRio(temp));
                    print(temp);
                }
            }
    }

    void verificaOeste(std::vector<NodoRio>& vecinos, Estado base, int pos) const {
        if(base.Oeste[pos] != 'X'){        
            Estado temp(base);
            temp = temp.swap(pos);
            if(temp.esValido()){
                vecinos.push_back(NodoRio(temp));
                print(temp);
            }
        }
    }
};


Heuristica<Estado> heuristica_Rio = [](Nodo<Estado> *fuente, Nodo<Estado> *destino) {
    return 1;
};

int main(){
    NodoRio* nodo_fuente = new NodoRio(
        Estado(std::array<char, 4>{'G','P','C','R'}, std::array<char, 4>{'X','X','X','X'})
    );

    NodoRio* nodo_destino = new NodoRio(
        Estado(std::array<char, 4>{'X','X','X','X'}, std::array<char, 4>{'G','P','C','R'})
    );

     auto camino = resolver_a_star(nodo_fuente, nodo_destino, heuristica_Rio);

       std::cout << "Camino:\n";
    for (const auto& nodo : camino)
    {
        //std::cout << *dynamic_cast<Nodo8Puzzle*>(nodo) << '\n';
    }
}