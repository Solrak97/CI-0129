cmake_minimum_required (VERSION 3.8)

project("AStar")

set(CMAKE_CXX_STANDARD 17)

include_directories("include")

add_executable("grafo"
	"include/a_star.h"
	"src/grafo.cpp")

add_executable("8_puzzle"
	"include/a_star.h"
	"src/8_puzzle.cpp")

# Aquí puede agregar la implementación de sus soluciones, como los ejemplos anteriores.
