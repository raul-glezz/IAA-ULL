/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   performance_analyzer.h
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Declaración de la clase PerformanceAnalyzer, que proporciona funcionalidades para analizar 
 *         el rendimiento del motor de inferencia condicional bajo diferentes configuraciones de consultas.
 */

#pragma once

#include "../conditional_inference_engine/conditional_inference_engine.h"
#include "../distribution/binary_distribution/binary_distribution.h"
#include "../conditional_query/conditional_query.h"
#include <vector>
#include <string>
#include <fstream>

/**
 * @brief Estructura para almacenar un punto de medición de rendimiento
 */
struct PerformanceDataPoint {
  /// number_interest_variables: número de variables de interés en la consulta
  int number_interest_variables;
  /// number_conditioned_variables: número de variables condicionadas en la consulta
  int number_conditioned_variables;
  /// number_marginalized_variables: número de variables a marginalizar en la consulta
  int number_marginalized_variables;
  /// execution_time: tiempo de ejecución de la consulta (en milisegundos)
  double execution_time;
  /// states_evaluated: número de estados evaluados durante la ejecución de la consulta
  uint64_t states_evaluated;
  
  //------------------------------------------------------CONSTRUCTOR------------------------------------------------------
  PerformanceDataPoint(int interest, int conditioned, int marginalized, double execution_time, uint64_t states_evaluated) : 
    number_interest_variables(interest), 
    number_conditioned_variables(conditioned),
    number_marginalized_variables(marginalized), 
    execution_time(execution_time),
    states_evaluated(states_evaluated) {}
};

/**
 * @brief Estructura para almacenar estadísticas básicas de un conjunto de mediciones
 */
struct Statistics {
  /// mean: media de los valores
  double mean;
  /// minimum: valor mínimo
  double minimum;
  /// maximum: valor máximo
  double maximum;
  /// standard_deviation: desviación estándar de los valores
  double standard_deviation;
};

class PerformanceAnalyzer {
 public:
  /// Método para ejecutar un análisis de rendimiento en una distribución y consulta dadas
  void runAnalysis(const BinaryDistribution&, int, int, int = 5);
  
  /// Método para obtener los puntos de medición de rendimiento almacenados
  const std::vector<PerformanceDataPoint>& getMeasurements() const { return measurements_; }
  /// Método para agregar un nuevo punto de medición de rendimiento a la lista de mediciones
  void addMeasurement(const BinaryDistribution&, const ConditionalQuery&);

  /// Método para exportar los puntos de medición de rendimiento a un archivo CSV
  void exportToCSV(const std::string&) const;
  /// Método para mostrar estadísticas básicas de los puntos de medición de rendimiento almacenados
  void displayStatistics() const;
  /// Método para generar un informe detallado de los puntos de medición de rendimiento almacenados
  void generateReport(const std::string&) const;

  /// Método para limpiar los puntos de medición de rendimiento almacenados
  void clear() { measurements_.clear(); }

 private:
  //-----------------ATRIBUTOS---------------------
  /// measurements_: vector para almacenar los puntos de medición de rendimiento
  std::vector<PerformanceDataPoint> measurements_;
  
  //-----------------------MÉTODO PRIVADO-----------------------
  /// Método para calcular estadísticas básicas (media, mínimo, máximo, desviación estándar)
  Statistics computeStatistics(const std::vector<double>&) const;
};
