/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   performance_analyzer.cc
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Definición de la clase PerformanceAnalyzer, que proporciona funcionalidades para analizar 
 *         el rendimiento del motor de inferencia condicional bajo diferentes configuraciones de consultas.
 */

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <random>
#include <map>

#include "performance_analyzer.h"

/**
 * @brief Método para ejecutar un análisis de rendimiento en una distribución y consulta dadas
 * @param[in] distribution: La distribución binaria sobre la que se realizarán las inferencias
 * @param[in] max_interest_variables: Número máximo de variables de interés a probar en las consultas
 * @param[in] max_conditioned_variables: Número máximo de variables condicionadas a probar en las consultas
 * @param[in] repetitions: Número de repeticiones para cada configuración de consulta
 */
void PerformanceAnalyzer::runAnalysis(const BinaryDistribution& distribution, int max_interest_variables, int max_conditioned_variables, int repetitions) {
  clear();
  
  int number_variables = distribution.getNumberVariables();
  ConditionalInferenceEngine engine(distribution);
  
  std::random_device rd;
  std::mt19937 gen(rd());
  
  int test_count = 0;
  int total_tests = max_interest_variables * max_conditioned_variables * repetitions;
  
  // Variamos el número de variables de interés
  for (int number_interest = 1; number_interest <= max_interest_variables && number_interest < number_variables; ++number_interest) {
    // Variamos el número de variables condicionadas
    for (int number_conditioned = 0; number_conditioned <= max_conditioned_variables && (number_interest + number_conditioned) < number_variables; ++number_conditioned) {
      // Repetimos para promediar
      for (int repetition = 0; repetition < repetitions; ++repetition) {
        test_count++;
        
        // Creamos una consulta aleatoria
        ConditionalQuery query(number_variables);
        
        // Seleccionamos las variables aleatorias sin repetición
        std::vector<int> available_variables(number_variables);
        std::iota(available_variables.begin(), available_variables.end(), 0);
        std::shuffle(available_variables.begin(), available_variables.end(), gen);
        
        // Añadimos las variables de interés
        for (int i = 0; i < number_interest; ++i) {
          query.addInterestVariable(available_variables[i]);
        }
        
        // Añadimos las variables condicionadas con valores aleatorios
        std::uniform_int_distribution<> dis(0, 1);
        for (int i = 0; i < number_conditioned; ++i) {
          int value = dis(gen);
          query.addConditionedVariable(available_variables[number_interest + i], value);
        }
        
        query.computeMasks();
        
        // Ejecutamos la inferencia y medimos los resultados
        auto result = engine.computeConditional(query);
        
        measurements_.emplace_back(
          number_interest,
          number_conditioned,
          query.getNumberMarginalizedVariables(),
          result.execution_time,
          result.states_evaluated
        );
      }
    }
  }
}

/**
 * @brief Método para agregar un nuevo punto de medición de rendimiento a la lista de mediciones
 * @param[in] distribution: La distribución binaria sobre la que se realizará la inferencia
 * @param[in] query: La consulta condicional para la cual se medirá el rendimiento
 */
void PerformanceAnalyzer::addMeasurement(const BinaryDistribution& distribution, const ConditionalQuery& query) {
  ConditionalInferenceEngine engine(distribution);
  auto result = engine.computeConditional(query);
  
  measurements_.emplace_back(
    query.getNumberInterestVariables(),
    query.getNumberConditionedVariables(),
    query.getNumberMarginalizedVariables(),
    result.execution_time,
    result.states_evaluated
  );
}

/**
 * @brief Método para exportar los puntos de medición de rendimiento a un archivo CSV
 * @param[in] filename: El nombre del archivo CSV donde se exportarán los datos
 */
void PerformanceAnalyzer::exportToCSV(const std::string& filename) const {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Error: No se pudo abrir el archivo para escritura " + filename);
  }
  
  file << "VariablesInteres,VariablesCondicionadas,VariablesMarginalizadas,TiempoEjecucion(us),EstadosEvaluados\n";
  
  for (const auto& point : measurements_) {
    file << point.number_interest_variables << ","
         << point.number_conditioned_variables << ","
         << point.number_marginalized_variables << ","
         << std::fixed << std::setprecision(2) << point.execution_time << ","
         << point.states_evaluated << "\n";
  }
  
  file.close();
}

/**
 * @brief Método para mostrar estadísticas básicas de los puntos de medición de rendimiento almacenados
 */
void PerformanceAnalyzer::displayStatistics() const {
  if (measurements_.empty()) {
    std::cout << "No hay mediciones disponibles." << std::endl;
    return;
  }
  
  // Agrupamos por pares de (variables de interés, variables condicionadas)
  std::map<std::pair<int, int>, std::vector<double>> groups;
  
  for (const auto& measurement : measurements_) {
    groups[{measurement.number_interest_variables, measurement.number_conditioned_variables}].push_back(measurement.execution_time);
  }
  
  std::cout << std::setw(12) << "Interés" 
            << std::setw(14) << "Condicionado"
            << std::setw(12) << "Marginal"
            << std::setw(15) << "Media(us)"
            << std::setw(15) << "Mín(us)"
            << std::setw(15) << "Máx(us)"
            << std::setw(15) << "DesvEst(us)"
            << std::endl;
  std::cout << std::string(90, '-') << std::endl;
  
  for (const auto& [key, times] : groups) {
    auto stats = computeStatistics(times);
    // Asumimos que el número de variables marginalizadas es constante para cada grupo, por lo que tomamos el valor del primer punto de medición
    int marginalized = measurements_[0].number_marginalized_variables;
    
    std::cout << std::setw(10) << key.first
              << std::setw(12) << key.second
              << std::setw(12) << marginalized
              << std::setw(15) << std::fixed << std::setprecision(2) << stats.mean
              << std::setw(15) << stats.minimum
              << std::setw(15) << stats.maximum
              << std::setw(15) << stats.standard_deviation
              << std::endl;
  }
}

/**
 * @brief Método para generar un informe detallado de los puntos de medición de rendimiento almacenados
 * @param[in] filename: El nombre del archivo donde se guardará el informe
 */
void PerformanceAnalyzer::generateReport(const std::string& filename) const {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Error: No se pudo abrir el archivo para escritura " + filename);
  }
  
  file << "Mediciones totales: " << measurements_.size() << std::endl;
  file << std::endl;
  
  // Mostramos estadísticas generales
  std::vector<double> allTimes;
  for (const auto& measurement : measurements_) {
    allTimes.push_back(measurement.execution_time);
  }
  
  auto overallStats = computeStatistics(allTimes);
  
  file << "Estadísticas generales:\n";
  file << "  Tiempo medio de ejecución: " << overallStats.mean << " us" << std::endl;
  file << "  Tiempo mínimo de ejecución:  " << overallStats.minimum << " us" << std::endl;
  file << "  Tiempo máximo de ejecución:  " << overallStats.maximum << " us" << std::endl;
  file << "  Desviación estándar:       " << overallStats.standard_deviation << " us" << std::endl;
  file << std::endl;
  
  // Mostramos estadísticas por configuración
  file << "Análisis detallado por configuración:" << std::endl;
  file << std::string(80, '-') << std::endl;
  
  std::map<std::pair<int, int>, std::vector<double>> groups;
  for (const auto& measurement : measurements_) {
    groups[{measurement.number_interest_variables, measurement.number_conditioned_variables}].push_back(measurement.execution_time);
  }
  
  for (const auto& [key, times] : groups) {
    auto stats = computeStatistics(times);
    file << "\nVariables de interés: " << key.first 
          << ", Variables condicionadas: " << key.second << std::endl;
    file << "  Muestras: " << times.size() << std::endl;
    file << "  Media: " << stats.mean << " us" << std::endl;
    file << "  Mín:  " << stats.minimum << " us" << std::endl;
    file << "  Máx:  " << stats.maximum << " us" << std::endl;
    file << "  DesvEst: " << stats.standard_deviation << " us" << std::endl;
  }
  
  file.close();
}

/**
 * @brief Método para calcular estadísticas básicas (media, mínimo, máximo, desviación estándar)
 * @param[in] values: Un vector de valores numéricos para los cuales se calcularán las estadísticas
 * @return Una estructura Statistics que contiene la media, mínimo, máximo y desviación estándar de los valores
 */
Statistics PerformanceAnalyzer::computeStatistics(const std::vector<double>& values) const {
  Statistics stats;
  
  if (values.empty()) {
    stats.mean = stats.minimum = stats.maximum = stats.standard_deviation = 0.0;
    return stats;
  }
  
  stats.mean = std::accumulate(values.begin(), values.end(), 0.0) / values.size();
  stats.minimum = *std::min_element(values.begin(), values.end());
  stats.maximum = *std::max_element(values.begin(), values.end());
  
  double variance = 0.0;
  for (double v : values) {
    variance += (v - stats.mean) * (v - stats.mean);
  }
  variance /= values.size();
  stats.standard_deviation = std::sqrt(variance);
  
  return stats;
}
