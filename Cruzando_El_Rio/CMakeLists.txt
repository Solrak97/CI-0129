cmake_minimum_required (VERSION 3.8)

project("AStar")

set(CMAKE_CXX_STANDARD 17)

include_directories("include")

add_executable("CruzandoElRio"
	"include/a_star.h"
	"src/cruzandoRio.cpp")	

# Aquí puede agregar la implementación de sus soluciones, como los ejemplos anteriores.
