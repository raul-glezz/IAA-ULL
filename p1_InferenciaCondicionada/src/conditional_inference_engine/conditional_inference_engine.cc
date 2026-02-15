/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   conditional_inference_engine.cc
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Implementación de la clase ConditionalInferenceEngine, 
 *         que implementa el motor de inferencia para distribuciones binarias,
 *         permitiendo calcular distribuciones condicionales a partir de una distribución conjunta.
 */

#include <cstring>
#include <stdexcept>
#include <numeric>
#include "conditional_inference_engine.h"

/**
 * @brief Constructor del motor de inferencia
 * @param[in] joint_distribution: Distribución conjunta sobre la que realizar inferencias
 */
ConditionalInferenceEngine::ConditionalInferenceEngine(const BinaryDistribution& joint_distribution) : 
  jointDistribution_(joint_distribution) {}

/**
 * @brief Método para calcular la distribución condicional P(X_I | X_C = c) usando marginalización
 * @param[in] maskC: Máscara de variables condicionadas
 * @param[in] valC: Valores de variables condicionadas
 * @param[in] maskI: Máscara de variables de interés
 * @return Array con probabilidades condicionales
 */
double* ConditionalInferenceEngine::prob_cond_bin(uint64_t maskC, uint64_t valC, uint64_t maskI) {
  int number_interest_bits = countBits(maskI);
  uint64_t interest_states = 1ULL << number_interest_bits;

  // Asignamos memoria para el resultado
  double* output = new double[interest_states];
  std::memset(output, 0, interest_states * sizeof(double));

  uint64_t total_states = jointDistribution_.getStateSpaceSize();
  // Iteramos sobre todos los estados para acumular probabilidades consistentes con las condiciones
  for (uint64_t state = 0; state < total_states; ++state) {
    if (isConsistent(state, maskC, valC)) {
      uint64_t interest_idx = extractInterestBits(state, maskI);
      output[interest_idx] += jointDistribution_.getProbability(state);
    }
  }

  // Normalizamos el resultado para obtener una distribución válida
  double sum = 0.0;
  for (uint64_t i = 0; i < interest_states; ++i) { sum += output[i]; }
  if (sum > 1e-10) {
    for (uint64_t i = 0; i < interest_states; ++i) {
      output[i] /= sum;
    }
  }

  return output;
}

/**
 * @brief Método principal para calcular la distribución condicional P(X_I | X_C = c)
 * @param[in] query: Consulta condicional que especifica las variables de interés y condicionadas
 * @return Estructura con la distribución condicional resultante y métricas de ejecución
 */
InferenceResult ConditionalInferenceEngine::computeConditional(const ConditionalQuery& query) {
  InferenceResult result;

  auto start = std::chrono::high_resolution_clock::now();

  double* output = prob_cond_bin(query.getMaskC(), query.getValC(), query.getMaskI());
  int number_interest_bits = countBits(query.getMaskI());
  uint64_t interest_states = 1ULL << number_interest_bits;
  
  // Creamos la distribución condicional resultante a partir del array de probabilidades
  auto distribution = std::make_unique<BinaryDistribution>(number_interest_bits);
  for (uint64_t i = 0; i < interest_states; ++i) { distribution->setProbability(i, output[i]); }
  delete[] output;

  auto end = std::chrono::high_resolution_clock::now();
  result.execution_time = std::chrono::duration<double, std::micro>(end - start).count();
  result.states_evaluated = jointDistribution_.getStateSpaceSize();
  result.distribution = std::move(distribution);

  return result;
}

/**
 * @brief Método para verificar si una configuración es consistente con las condiciones
 * @param[in] state Estado completo
 * @param[in] maskC Máscara de variables condicionadas
 * @param[in] valC Valores esperados
 */
bool ConditionalInferenceEngine::isConsistent(uint64_t state, uint64_t maskC, uint64_t valC) const {
  // Un estado es consistente si los bits marcados en maskC tienen los valores de valC
  return (state & maskC) == valC;
}

/**
 * @brief Método para extraer los bits de interés de un estado dado una máscara
 * @param[in] state: Estado completo
 * @param[in] maskI: Máscara de variables de interés
 */
uint64_t ConditionalInferenceEngine::extractInterestBits(uint64_t state, uint64_t maskI) const {
  // Extraer solo los bits marcados en maskI y compactarlos
  uint64_t result = 0;
  int resultBit = 0;
  
  for (int bit = 0; bit < 64; ++bit) {
    if (maskI & (1ULL << bit)) {
      if (state & (1ULL << bit)) {
        result |= (1ULL << resultBit);
      }
      resultBit++;
    }
  }
  
  return result;
}

/**
 * @brief Método para contar el número de bits a 1 en una máscara
 * @param[in] mask: Máscara a analizar
 * @return Número de bits a 1 en la máscara
 */
int ConditionalInferenceEngine::countBits(uint64_t mask) const {
  int count = 0;
  while (mask) {
    count += mask & 1;
    mask >>= 1;
  }
  return count;
}
