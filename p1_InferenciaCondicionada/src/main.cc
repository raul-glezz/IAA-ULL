/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   main.cc
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Programa principal para el cálculo de distribuciones condicionales 
 *         a partir de una distribución conjunta binaria,
 */

#include <iostream>
#include <exception>

#include "user_interface/user_interface.h"

int main(int argc, char* argv[]) {
  try {
    UserInterface user_interface;
    user_interface.run();
    return EXIT_SUCCESS;
  } catch (const std::exception& exception) {
    std::cerr << "\nError: " << exception.what() << std::endl;
    return EXIT_FAILURE;
  }
}