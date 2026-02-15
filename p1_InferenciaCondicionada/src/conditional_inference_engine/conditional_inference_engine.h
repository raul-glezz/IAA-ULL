/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   conditional_inference_engine.h
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Declaración de la clase ConditionalInferenceEngine, 
 *         que implementa el motor de inferencia para distribuciones binarias,
 *         permitiendo calcular distribuciones condicionales a partir de una distribución conjunta.
 */

#pragma once

#include <memory>
#include <chrono>
#include <vector>

#include "../distribution/binary_distribution/binary_distribution.h"
#include "../conditional_query/conditional_query.h"

struct InferenceResult {
  //------------------------------------CONSTRUCTOR------------------------------------
  InferenceResult() : distribution(nullptr), execution_time(0.0), states_evaluated(0) {}
  
  /// distribution: Distribución condicional resultante
  std::unique_ptr<BinaryDistribution> distribution;
  /// execution_time: Tiempo de ejecución del proceso de inferencia en segundos
  double execution_time;
  /// states_evaluated: Número de estados evaluados durante la inferencia
  uint64_t states_evaluated;
};

class ConditionalInferenceEngine {
 public:
  //-------------------------CONSTRUCTOR-------------------------
  explicit ConditionalInferenceEngine(const BinaryDistribution&);

  //-------------------------MÉTODOS-------------------------
  /// Método principal para calcular la distribución condicional P(X_I | X_C = c)
  InferenceResult computeConditional(const ConditionalQuery&);
  /// Método para calcular la distribución condicional P(X_I | X_C = c) usando marginalización
  double* prob_cond_bin(uint64_t, uint64_t, uint64_t);

 protected:
  //-----------------MÉTODOS PROTEGIDOS-----------------
  /// Método para verificar si una configuración es consistente con las condiciones
  bool isConsistent(uint64_t, uint64_t, uint64_t) const;
  /// Método para extraer el valor de las variables de interés de un estado
  uint64_t extractInterestBits(uint64_t, uint64_t) const;
  /// Método para contar el número de bits activos en una máscara
  int countBits(uint64_t) const;

 private:
  //-----------------ATRIBUTO-----------------
  /// jointDistribution_: Referencia a la distribución conjunta sobre la que se realizarán las inferencias
  const BinaryDistribution& jointDistribution_;
};