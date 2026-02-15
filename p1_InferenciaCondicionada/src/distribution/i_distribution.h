/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   i_distribution.h
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Implementación de la clase base IDistribution 
 *         para distribuciones de probabilidad
 */

#pragma once

#include <vector>
#include <string>
#include <cstdint>

class IDistribution {
 public:
  //-----------DESTRUCTOR-----------
  virtual ~IDistribution() = default;
  
  //--------------------MÉTODOS--------------------
  /// Método que devuelve el número de variables en la distribución
  virtual int getNumberVariables() const = 0;
  /// Método que devuelve la probabilidad de un estado dado su índice
  virtual double getProbability(uint64_t) const = 0;
  /// Método que devuelve el tamaño del espacio de estados de la distribución
  virtual uint64_t getStateSpaceSize() const = 0;
  
  /// Método que establece la probabilidad de un estado dado su índice
  virtual void setProbability(uint64_t, double) = 0;
  
  /// Método que comprueba si la distribución es válida
  virtual bool isValid() const = 0;
  /// Método que normaliza la distribución
  virtual void normalize() = 0;
  
  /// Método que muestra la distribución por pantalla
  virtual void display() const = 0;
  /// Método que exporta la distribución a un archivo CSV
  virtual void exportToCSV(const std::string&) const = 0;
};