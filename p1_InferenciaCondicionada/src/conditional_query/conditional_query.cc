/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   conditional_query.cc
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Definición de la clase ConditionalQuery, 
 *         que representa una consulta condicional sobre variables binarias discretas.
 */

#include <algorithm>
#include <iomanip>
#include <bitset>

#include "conditional_query.h"

/**
 * @brief Constructor que inicializa la consulta con un número dado de variables
 * @param[in] number_variables: Número total de variables en la consulta
 * @throws std::invalid_argument si el número de variables es menor o igual a 0 o mayor a 64
 */
ConditionalQuery::ConditionalQuery(int number_variables) :
  number_variables_(number_variables), maskC_(0), valC_(0), maskI_(0), maskM_(0) {
  if (number_variables <= 0 || number_variables > 64) {
    throw std::invalid_argument("El número de variables debe estar entre 1 y 64");
  }
}

/**
 * @brief Método para añadir una variable condicionada a la consulta
 * @param[in] variable_index: Índice de la variable a condicionar (0-based)
 * @param[in] value: Valor de la variable condicionada (0 o 1)
 * @throws std::invalid_argument si el índice de variable está fuera del rango permitido,
 *         si el valor no es binario, o si la variable ya está en condicionadas
 */
void ConditionalQuery::addConditionedVariable(int variable_index, int value) {
  validateVariableIndex(variable_index);
  validateValue(value);
  
  // Verificamos que no esté duplicada
  auto it = std::find(conditioned_variables_.begin(), conditioned_variables_.end(), variable_index);
  if (it != conditioned_variables_.end()) {
    throw std::invalid_argument("Variable ya condicionada: " + std::to_string(variable_index));
  }
  
  // Verificamos que no esté entre las variables de interés
  it = std::find(interest_variables_.begin(), interest_variables_.end(), variable_index);
  if (it != interest_variables_.end()) {
    throw std::invalid_argument("Variable ya en el conjunto de interés: " + std::to_string(variable_index));
  }
  
  conditioned_variables_.push_back(variable_index);
  conditioned_values_.push_back(value);
}

/**
 * @brief Método para añadir una variable de interés a la consulta
 * @param[in] variable_index: Índice de la variable de interés (0-based)
 * @throws std::invalid_argument si el índice de variable está fuera del rango permitido,
 *         o si la variable ya está en interés o condicionadas
 */
void ConditionalQuery::addInterestVariable(int variable_index) {
  validateVariableIndex(variable_index);
  
  // Verificamos que no esté duplicada
  auto it = std::find(interest_variables_.begin(), interest_variables_.end(), variable_index);
  if (it != interest_variables_.end()) {
    throw std::invalid_argument("Variable ya en el conjunto de interés: " + std::to_string(variable_index));
  }
  
  // Verificamos que no esté en variables condicionadas
  it = std::find(conditioned_variables_.begin(), conditioned_variables_.end(), variable_index);
  if (it != conditioned_variables_.end()) {
    throw std::invalid_argument("Variable ya condicionada: " + std::to_string(variable_index));
  }
  
  interest_variables_.push_back(variable_index);
}

/**
 * @brief Método para calcular las máscaras de la consulta 
 *        a partir de las variables condicionadas e interés
 */
void ConditionalQuery::computeMasks() {
  maskC_ = 0;
  valC_ = 0;
  maskI_ = 0;
  maskM_ = 0;
  
  // Calculamos la máscara y los valores de variables condicionadas
  for (size_t i = 0; i < conditioned_variables_.size(); ++i) {
    int varIdx = conditioned_variables_[i];
    int value = conditioned_values_[i];
    
    maskC_ |= (1ULL << varIdx);
    if (value == 1) { valC_ |= (1ULL << varIdx); }
  }
  
  // Calculamos la máscara de variables de interés
  for (int varIdx : interest_variables_) {
    maskI_ |= (1ULL << varIdx);
  }
  
  // Calculamos la máscara de variables a marginalizar
  uint64_t allMask = (1ULL << number_variables_) - 1;
  maskM_ = allMask & ~(maskC_ | maskI_);
}

/**
 * @brief Método para obtener el número de variables a marginalizar
 * @return Número de variables que no están ni en condicionadas ni en interés
 */
int ConditionalQuery::getNumberMarginalizedVariables() const {
  int total = number_variables_;
  int used = conditioned_variables_.size() + interest_variables_.size();
  return total - used;
}

/**
 * @brief Método para validar que la consulta sea consistente
 * @return True si la consulta es válida, false en caso contrario
 */
bool ConditionalQuery::isValid() const {
  // Debe haber al menos una variable de interés
  if (interest_variables_.empty()) { return false; }
  // No puede haber variables repetidas entre condicionadas e interés
  if (conditioned_variables_.size() != conditioned_values_.size()) { return false; }
  
  return true;
}

/**
 * @brief Método para obtener una representación en cadena de la consulta
 * @return String que describe la consulta, incluyendo las variables de interés, condicionadas y sus máscaras
 */
std::string ConditionalQuery::toString() const {
  std::ostringstream oss;
  
  oss << "Consulta: P(";
  
  // Imprimimos las variables de interés
  for (size_t i = 0; i < interest_variables_.size(); ++i) {
    if (i > 0) oss << ", ";
    oss << "X" << (interest_variables_[i] + 1);
  }
  
  // Imprimimos las variables condicionadas
  if (!conditioned_variables_.empty()) {
    oss << " | ";
    
    // Variables condicionadas
    for (size_t i = 0; i < conditioned_variables_.size(); ++i) {
      if (i > 0) oss << ", ";
      oss << "X" << (conditioned_variables_[i] + 1) << "=" << conditioned_values_[i];
    }
  }
  
  oss << ")" << std::endl;
  
  // Mostramos las máscaras en formato binario
  oss << "  maskC: " << std::bitset<64>(maskC_).to_string().substr(64 - number_variables_) << std::endl;
  oss << "  valC:  " << std::bitset<64>(valC_).to_string().substr(64 - number_variables_) << std::endl;
  oss << "  maskI: " << std::bitset<64>(maskI_).to_string().substr(64 - number_variables_) << std::endl;
  oss << "  maskM: " << std::bitset<64>(maskM_).to_string().substr(64 - number_variables_) << std::endl;
  
  return oss.str();
}

/**
 * @brief Método para validar que el índice de variable esté dentro del rango permitido
 * @param[in] variable_index: Índice de la variable a validar
 * @throws std::invalid_argument si el índice está fuera del rango [0, number_variables_-1]
 */
void ConditionalQuery::validateVariableIndex(int variable_index) const {
  if (variable_index < 0 || variable_index >= number_variables_) {
    throw std::invalid_argument("Variable fuera de rango: " + 
          std::to_string(variable_index) + " (Debe ser 0-" + std::to_string(number_variables_ - 1) + ")");
  }
}

/**
 * @brief Método para validar que el valor de variable sea binario
 * @param[in] value: Valor a validar
 * @throws std::invalid_argument si el valor no es 0 o 1
 */
void ConditionalQuery::validateValue(int value) const {
  if (value != 0 && value != 1) {
    throw std::invalid_argument("El valor debe ser 0 o 1, se recibió: " + std::to_string(value));
  }
}
