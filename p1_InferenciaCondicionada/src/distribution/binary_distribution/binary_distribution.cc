/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   binary_distribution.cc
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Implementación de la clase BinaryDistribution, 
 *         que representa una distribución conjunta de variables binarias discretas.
 */

#include <random>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

#include "binary_distribution.h"

/**
 * @brief Constructor que inicializa la distribución con un número dado de variables
 * @param[in] number_variables: Número de variables binarias (N)
 * @throws std::invalid_argument si el número de variables es menor o igual a 0 o mayor a 64
 */
BinaryDistribution::BinaryDistribution(int number_variables) : number_variables_(number_variables) {
  if (number_variables <= 0 || number_variables > 64) {
    throw std::invalid_argument("Error: El número de variables debe estar entre 1 y 64");
  }
    
  state_space_size_ = 1ULL << number_variables_;
  probabilities_.resize(state_space_size_, 0.0);
}

/**
 * @brief Constructor que carga la distribución desde un archivo CSV
 * @param[in] filename: Ruta del archivo CSV
 * @throws std::runtime_error si hay errores al leer el archivo o al parsear su contenido
 */
BinaryDistribution::BinaryDistribution(const std::string& filename) {
  loadFromCSV(filename);
}

/**
 * @brief Método para obtener la probabilidad de una configuración específica
 * @param[in] index: Índice de la configuración (codificación binaria)
 * @return Probabilidad asociada a la configuración dada por el índice
 * @throws std::out_of_range si el índice está fuera del rango válido
 */
double BinaryDistribution::getProbability(uint64_t index) const {
  if (index >= state_space_size_) {
    throw std::out_of_range("Error: Índice fuera de rango");
  }
  return probabilities_[index];
}

/**
 * @brief Método para establecer la probabilidad de una configuración específica
 * @param[in] index: Índice de la configuración (codificación binaria)
 * @param[in] probability: Probabilidad a asignar
 * @throws std::out_of_range si el índice está fuera del rango válido
 * @throws std::invalid_argument si la probabilidad no está en el rango [0, 1]
 */
void BinaryDistribution::setProbability(uint64_t index, double probability) {
  if (index >= state_space_size_) {
    throw std::out_of_range("Error: Índice fuera de rango");
  }
  if (probability < 0.0 || probability > 1.0) {
    throw std::invalid_argument("Error: La probabilidad debe estar entre 0 y 1");
  }
  probabilities_[index] = probability;
}

/**
 * @brief Método para validar que la distribución esté correctamente normalizada
 * @return true si la suma de las probabilidades es 1 (dentro de una tolerancia), false en caso contrario
 */
bool BinaryDistribution::isValid() const {
  double sum = 0.0;
  for (double probability : probabilities_) {
    if (probability < -EPSILON || probability > 1.0 + EPSILON) {
      return false;
    }
    sum += probability;
  }
  // Comprobamos que la suma de probabilidades sea 1 dentro de una tolerancia
  return std::abs(sum - 1.0) < EPSILON;
}

/**
 * @brief Método para normalizar la distribución
 * @throws std::runtime_error si la suma de las probabilidades es cero (no se puede normalizar)
 */
void BinaryDistribution::normalize() {
  double sum = 0.0;
  for (double probability : probabilities_) {
    sum += probability;
  }
  
  if (sum < EPSILON) {
    throw std::runtime_error("Error: No se puede normalizar, la suma de probabilidades es cero");
  }
  
  // Normalizamos cada probabilidad dividiéndola por la suma total
  for (double& probability : probabilities_) {
    probability /= sum;
  }
}

/**
 * @brief Método para mostrar la distribución en consola
 */
void BinaryDistribution::display() const {
  std::cout << "=== Distribución Binaria (N=" << number_variables_ << ") ===" << std::endl;
  std::cout << std::fixed << std::setprecision(6);
  
  // Imprimimos el encabezado de la tabla
  std::cout << "Config   |";
  for (int i = number_variables_ - 1; i >= 0; i--) {
    std::cout << " X" << (i + 1);
  }
  std::cout << " | Probabilidad" << std::endl;
  std::cout << std::string(10 + number_variables_ * 3 + 15, '-') << std::endl;
  
  for (uint64_t i = 0; i < state_space_size_; ++i) {
    // Mostramos solo las configuraciones con probabilidad > 0
    if (probabilities_[i] > EPSILON) {
      std::string binary = indexToBinary(i);
      std::cout << std::setw(8) << binary << " |";
      for (char bit : binary) { std::cout << "  " << bit; }
      std::cout << " | " << std::setw(10) << probabilities_[i] << std::endl;
    }
  }
  std::cout << std::endl;

  std::cout << "Suma total de probabilidades: " << std::accumulate(probabilities_.begin(), probabilities_.end(), 0.0) << std::endl;
}

/**
 * @brief Método para exportar la distribución a un archivo CSV
 * @param[in] filename: Ruta del archivo CSV de salida
 * @throws std::runtime_error si no se puede abrir el archivo
 */
void BinaryDistribution::exportToCSV(const std::string& filename) const {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Error: No se puede abrir el archivo " + filename);
  }
  
  file << std::fixed << std::setprecision(10);
  for (uint64_t i = 0; i < state_space_size_; ++i) {
    file << indexToBinary(i) << "," << probabilities_[i] << std::endl;
  }
  
  file.close();
}

/**
 * @brief Método para generar una distribución aleatoria
 */
void BinaryDistribution::generateRandom() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.1, 10.0);
  
  for (uint64_t i = 0; i < state_space_size_; i++) {
    probabilities_[i] = dis(gen);
  }
  // Normalizamos después de generar los valores aleatorios
  normalize();
}

/**
 * @brief Método para convertir un índice numérico a su representación binaria
 * @param[in] index: Índice numérico a convertir
 * @return Cadena de texto con la representación binaria del índice, con ceros a la izquierda
 */
std::string BinaryDistribution::indexToBinary(uint64_t index) const {
  std::string binary;
  binary.reserve(number_variables_);
  
  for (int i = 0; i < number_variables_; ++i) {
    binary = (index & 1 ? '1' : '0') + binary;
    index >>= 1;
  }
  
  return binary;
}

/**
 * @brief Método que carga la distribución desde un archivo CSV
 * @param[in] filename: Ruta del archivo CSV
 * @throws std::runtime_error si hay errores al leer el archivo o al parsear su contenido
 */
void BinaryDistribution::loadFromCSV(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("No se puede abrir el archivo: " + filename);
  }
  
  std::string line;
  std::vector<std::pair<std::string, double>> data;
  
  // Leemos el archivo línea por línea
  while (std::getline(file, line)) {
    if (line.empty()) continue;
    
    // Separamos la máscara binaria y la probabilidad usando la coma como delimitador
    size_t commaPos = line.find(',');
    if (commaPos == std::string::npos) {
      throw std::runtime_error("Formato CSV inválido: " + line);
    }
    
    std::string binary = line.substr(0, commaPos);
    double probability = std::stod(line.substr(commaPos + 1));
    
    data.push_back({binary, probability});
  }
  
  file.close();
  
  // Si el archivo está vacío, no podemos cargar una distribución válida
  if (data.empty()) {
    throw std::runtime_error("Archivo CSV vacío");
  }
  
  // Determinamos el número de variables a partir de la longitud de la primera máscara binaria
  number_variables_ = data[0].first.length();
  state_space_size_ = 1ULL << number_variables_;
  probabilities_.resize(state_space_size_, 0.0);
  
  // Cargamos las probabilidades en el vector, validando la longitud de cada máscara binaria
  for (const auto& [binary, probability] : data) {
    if (binary.length() != static_cast<size_t>(number_variables_)) {
      throw std::runtime_error("Longitud de máscara inconsistente en CSV");
    }
    
    uint64_t index = binaryToIndex(binary);
    probabilities_[index] = probability;
  }
}

/**
 * @brief Método que convierte la máscara binaria a su índice numérico
 * @param[in] binary: Cadena de texto con la representación binaria
 * @return Índice numérico correspondiente a la máscara binaria
 */
uint64_t BinaryDistribution::binaryToIndex(const std::string& binary) const {
  uint64_t index = 0;
  for (char bit : binary) {
    index = (index << 1) | (bit == '1' ? 1 : 0);
  }
  return index;
}
