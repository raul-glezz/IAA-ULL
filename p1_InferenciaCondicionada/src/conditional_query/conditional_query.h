/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   conditional_query.h
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Declaración de la clase ConditionalQuery, 
 *         que representa una consulta condicional sobre variables binarias discretas.
 */

#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <sstream>

class ConditionalQuery {
 public:
  //---------CONSTRUCTOR---------
  explicit ConditionalQuery(int);

  //-----------------------------------------MÉTODOS-----------------------------------------
  /// Método para obtener la máscara de variables condicionadas
  uint64_t getMaskC() const { return maskC_; }
  /// Método para obtener la máscara de variables de interés
  uint64_t getMaskI() const { return maskI_; }
  /// Método para obtener la máscara de variables a marginalizar
  uint64_t getMaskM() const { return maskM_; }
  /// Método para obtener los valores de las variables condicionadas
  uint64_t getValC() const { return valC_; }
  
  /// Método para obtener las variables condicionadas
  const std::vector<int>& getConditionedVariables() const { return conditioned_variables_; }
  /// Método para obtener los valores de las variables condicionadas
  const std::vector<int>& getConditionedValues() const { return conditioned_values_; }
  /// Método para obtener las variables de interés
  const std::vector<int>& getInterestVariables() const { return interest_variables_; }
  
  /// Método para obtener el número de variables condicionadas
  int getNumberConditionedVariables() const { return conditioned_variables_.size(); }
  /// Método para obtener el número de variables de interés
  int getNumberInterestVariables() const { return interest_variables_.size(); }
  /// Método para obtener el número de variables a marginalizar
  int getNumberMarginalizedVariables() const;
  
  /// Método para agregar una variable condicionada
  void addConditionedVariable(int, int);
  /// Método para agregar una variable de interés
  void addInterestVariable(int);
  
  /// Método para calcular las máscaras de la consulta
  void computeMasks();
  /// Método para validar que la consulta sea consistente
  bool isValid() const;
  /// Método para obtener una representación en cadena de la consulta
  std::string toString() const;
    
private:
  /// number_variables_: número total de variables
  int number_variables_;
  /// conditioned_variables_: índices de variables condicionadas
  std::vector<int> conditioned_variables_;
  /// conditioned_values_: valores de variables condicionadas
  std::vector<int> conditioned_values_;
  /// interest_variables_: índices de variables de interés
  std::vector<int> interest_variables_;
  /// maskC_: máscara de variables condicionadas
  uint64_t maskC_;
  /// valC_: valores de variables condicionadas
  uint64_t valC_;
  /// maskI_: máscara de variables de interés
  uint64_t maskI_;
  /// maskM_: máscara de variables a marginalizar
  uint64_t maskM_;

  //---------MÉTODOS PRIVADOS-----------
  /// Método para validar que el índice de variable esté dentro del rango permitido
  void validateVariableIndex(int) const;
  /// Método para validar que el valor de variable sea binario
  void validateValue(int) const;
};
