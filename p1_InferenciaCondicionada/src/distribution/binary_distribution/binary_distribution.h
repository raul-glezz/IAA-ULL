/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   binary_distribution.h
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Declaración de la clase BinaryDistribution, 
 *         que representa una distribución conjunta de variables binarias discretas.
 */

#pragma once

#include "../i_distribution.h"

// EPSILON: tolerancia para comparaciones de punto flotante
static constexpr double EPSILON = 1e-9;

/**
 * @brief Clase que representa una distribución conjunta de variables binarias discretas
 * Cada variable puede tomar valores 0 o 1, y la distribución asigna una probabilidad a cada combinación posible.
 */
class BinaryDistribution : public IDistribution {
 public:
  //----------------CONSTRUCTORES----------------
  explicit BinaryDistribution(int);
  explicit BinaryDistribution(const std::string&);

  //-----------------------------------MÉTODOS-----------------------------------
  /// Método que devuelve el número de variables en la distribución
  int getNumberVariables() const override { return number_variables_; }
  /// Método que devuelve el vector de probabilidades de la distribución
  const std::vector<double>& getProbabilities() const { return probabilities_; }
  /// Método que devuelve el tamaño del espacio de estados de la distribución
  uint64_t getStateSpaceSize() const override { return state_space_size_; }
  /// Método que devuelve la probabilidad de un estado dado su índice
  double getProbability(uint64_t) const override;

  /// Método que establece la probabilidad de un estado dado su índice
  void setProbability(uint64_t, double) override;

  /// Método que genera una distribución aleatoria válida
  void generateRandom();
  /// Método que normaliza la distribución para que la suma de probabilidades sea 1
  void normalize() override;
  /// Método que comprueba si la distribución es válida (suma de probabilidades = 1 y todas >= 0)
  bool isValid() const override;
  
  /// Método que convierte un índice de estado a su representación binaria como string
  std::string indexToBinary(uint64_t) const;
  /// Método que muestra la distribución por pantalla
  void display() const override;
  /// Método que exporta la distribución a un archivo CSV
  void exportToCSV(const std::string&) const override;
    
 private:
  //------------ATRIBUTOS------------
  /// number_variables_: número de variables en la distribución
  int number_variables_;
  /// probabilities_: vector que almacena la probabilidad de cada estado (combinación de variables)
  std::vector<double> probabilities_;
  /// state_space_size_: número total de combinaciones posibles de las variables (2^number_variables_)
  uint64_t state_space_size_;

  //----------------MÉTODOS PRIVADOS---------------
  /// Método para cargar la distribución desde un archivo CSV 
  void loadFromCSV(const std::string&);
  /// Método para convertir una representación binaria en string a su índice correspondiente
  uint64_t binaryToIndex(const std::string&) const;
};