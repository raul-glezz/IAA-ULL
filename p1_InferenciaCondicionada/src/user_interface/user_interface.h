/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   user_interface.h
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Declaración de la clase UserInterface, que proporciona una interfaz
 *         de línea de comandos para interactuar con el motor de inferencia condicional.
 */

#pragma once

#include <memory>
#include <iostream>

#include "../distribution/binary_distribution/binary_distribution.h"
#include "../conditional_query/conditional_query.h"
#include "../conditional_inference_engine/conditional_inference_engine.h"
#include "../performance_analyzer/performance_analyzer.h"

class UserInterface {
 public:
  //-----------------------------CONSTRUCTOR-----------------------------
  UserInterface() : analyzer_(std::make_unique<PerformanceAnalyzer>()) {}
  
  /// Método principal para ejecutar la interfaz de usuario
  void run();
    
 private:
  //--------------------ATRIBUTOS--------------------
  /// distribution_: Distribución conjunta binaria cargada o generada
  std::unique_ptr<BinaryDistribution> distribution_;
  /// engine_: Motor de inferencia condicional para realizar cálculos
  std::unique_ptr<ConditionalInferenceEngine> engine_;
  /// analyzer_: Analizador de rendimiento para evaluar el motor de inferencia
  std::unique_ptr<PerformanceAnalyzer> analyzer_;
  
  //---------------MÉTODOS PRIVADOS---------------
  /// Métodos para mostrar el menú principal
  void displayMainMenu() const;
  /// Métodos para cargar o generar distribuciones
  void loadOrGenerateDistribution();
  /// Método para crear una consulta condicional a partir de la entrada del usuario
  std::unique_ptr<ConditionalQuery> createQuery();
  /// Método para ejecutar la inferencia condicional y mostrar resultados
  void executeInference();
  /// Método para ejecutar el análisis de rendimiento
  void runPerformanceAnalysis();
  /// Método para mostrar la distribución actual por pantalla
  void displayCurrentDistribution();
  /// Método para exportar la distribución actual a un archivo CSV
  void exportDistribution();
  /// Método para mostrar la ayuda y explicación del sistema
  void displayHelp() const;
  
  /// Métodos auxiliares para leer el número de opción
  int readInt(const std::string&, int, int);
  /// Método auxiliar para leer una cadena de texto
  std::string readString(const std::string&);
  /// Método auxiliar para leer una confirmación (sí/no)
  bool readConfirmation(const std::string&);
  /// Método para verificar que una distribución esté cargada antes de realizar operaciones
  bool checkDistributionLoaded() const;
};
