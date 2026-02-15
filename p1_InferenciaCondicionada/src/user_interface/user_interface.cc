/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * Practica 1: Probabilidad Condicionada

 * @file   user_interface.cc
 * @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
 * @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
 * @date   05/02/2026
 * @brief  Definición de la clase UserInterface, que proporciona una interfaz
 *         de línea de comandos para interactuar con el motor de inferencia condicional.
 */

#include <limits>
#include <sstream>
#include "user_interface.h"

/**
 * @brief Ejecuta el bucle principal de la interfaz de usuario, mostrando el menú y procesando las opciones seleccionadas.
 */
void UserInterface::run() { 
  bool running = true;
  while (running) {
    displayMainMenu();
    
    int choice = readInt("Seleccione una opción", 0, 7);
    std::cout << std::endl;
    
    switch (choice) {
      case 1:
        loadOrGenerateDistribution();
        break;
      case 2:
        displayCurrentDistribution();
        break;
      case 3:
        executeInference();
        break;
      case 4:
        runPerformanceAnalysis();
        break;
      case 5:
        exportDistribution();
        break;
      case 6:
        displayHelp();
        break;
      case 0:
        running = false;
        break;
      default:
        std::cout << "Opción no válida. Por favor, elige una opción del menú." << std::endl;
    }
    
    if (running && choice != 0) {
      std::cout << "\nPulse Enter para continuar...";
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}

/**
 * @brief Muestra el menú principal de la interfaz de usuario.
 */
void UserInterface::displayMainMenu() const {
  std::cout << "1. Cargar o Generar Distribución" << std::endl;
  std::cout << "2. Mostrar Distribución Actual" << std::endl;
  std::cout << "3. Ejecutar Inferencia Condicional" << std::endl;
  std::cout << "4. Análisis de Rendimiento" << std::endl;
  std::cout << "5. Exportar Distribución a CSV" << std::endl;
  std::cout << "6. Ayuda" << std::endl;
  std::cout << "0. Salir" << std::endl;
}

/**
 * @brief Permite al usuario cargar una distribución desde un archivo CSV o generar una distribución aleatoria.
 */
void UserInterface::loadOrGenerateDistribution() {
  std::cout << "1. Cargar desde archivo CSV" << std::endl;
  std::cout << "2. Generar distribución aleatoria\n";
  int choice = readInt("Seleccione opción", 1, 2);
  
  if (choice == 1) {
    std::string filename = readString("\nIngrese el nombre del archivo CSV");
    
    try {
      distribution_ = std::make_unique<BinaryDistribution>(filename);
      engine_ = std::make_unique<ConditionalInferenceEngine>(*distribution_);
      
      std::cout << "  Variables: " << distribution_->getNumberVariables() << "\n";
      std::cout << "  Estados: " << distribution_->getStateSpaceSize() << "\n";
      
      // Si la distribución no es válida, la normalizamos
      if (!distribution_->isValid()) { distribution_->normalize(); }
    } catch (const std::exception& exception) {
      std::cout << "\nError al cargar la distribución: " << exception.what() << std::endl;
      distribution_.reset();
      engine_.reset();
    }
  } else {
    int number_variables = readInt("\nNúmero de variables (1-20)", 1, 20);
    
    try {
      distribution_ = std::make_unique<BinaryDistribution>(number_variables);
      distribution_->generateRandom();
      engine_ = std::make_unique<ConditionalInferenceEngine>(*distribution_);
      
      std::cout << "  Variables: " << number_variables << std::endl;
      std::cout << "  Estados: " << distribution_->getStateSpaceSize() << std::endl;
    } catch (const std::exception& exception) {
      std::cout << "\nError generando la distribución: " << exception.what() << std::endl;
      distribution_.reset();
      engine_.reset();
    }
  }
}

/**
 * @brief Crea una consulta condicional a partir de la entrada del usuario, solicitando las variables de interés y condicionadas.
 * @return Un puntero único a la consulta creada, o nullptr si no se pudo crear.
 */
std::unique_ptr<ConditionalQuery> UserInterface::createQuery() {
  if (!checkDistributionLoaded()) { return nullptr; }
  
  int number_variables = distribution_->getNumberVariables();
  auto query = std::make_unique<ConditionalQuery>(number_variables);
  
  std::cout << "\n--- Variables de interés ---\n";
  int number_interest_variables = readInt("Introduzca el número de variables de interés: ", 1, number_variables - 1);
  
  for (int i = 0; i < number_interest_variables; ++i) {
    int variable_index = readInt("  Índice de variable (1-" + std::to_string(number_variables) + ")", 1, number_variables) - 1;
    try {
      query->addInterestVariable(variable_index);
    } catch (const std::exception& exception) {
      std::cout << "  Error: " << exception.what() << std::endl;
      i--;
    }
  }
  
  std::cout << "\n--- Variables condicionadas ---\n";
  if (readConfirmation("¿Agregar variables condicionadas?")) {
    int maximum_conditioned_variables = number_variables - number_interest_variables;
    int number_conditioned_variables = readInt("Introduzca el número de variables condicionadas: ", 0, maximum_conditioned_variables);
    
    for (int i = 0; i < number_conditioned_variables; ++i) {
      int variable_index = readInt("  Índice de variable (1-" + std::to_string(number_variables) + ")", 1, number_variables) - 1;
      int value = readInt("  Valor (0 o 1)", 0, 1);
      
      try {
        query->addConditionedVariable(variable_index, value);
      } catch (const std::exception& exception) {
        std::cout << "  Error: " << exception.what() << std::endl;
        i--;
      }
    }
  }
  
  query->computeMasks();
  std::cout << std::endl;
  std::cout << query->toString();
  
  return query;
}

/**
 * @brief Ejecuta el proceso de inferencia condicional utilizando la consulta creada por el usuario, mostrando los resultados y métricas de ejecución.
 */
void UserInterface::executeInference() {
  if (!checkDistributionLoaded()) { return; }
  
  auto query = createQuery();
  if (!query || !query->isValid()) {
    std::cout << "\nConsulta inválida.\n";
    return;
  }
  
  try {
    auto result = engine_->computeConditional(*query);
    
    result.distribution->display();
    
    std::cout << "\n--- Métricas de ejecución ---\n";
    std::cout << "  Tiempo de ejecución: " << std::fixed << std::setprecision(2) << result.execution_time << " μs" << std::endl;
    std::cout << "  Estados evaluados: " << result.states_evaluated << std::endl;
    std::cout << "  Variables de interés: " << query->getNumberInterestVariables() << std::endl;
    std::cout << "  Variables condicionadas: " << query->getNumberConditionedVariables() << std::endl;
    std::cout << "  Variables marginalizadas: " << query->getNumberMarginalizedVariables() << std::endl;
    
    if (readConfirmation("\n¿Guardar resultado en CSV?")) {
      std::string filename = readString("Nombre del archivo de salida");
      result.distribution->exportToCSV(filename);
    }
  } catch (const std::exception& exception) {
    std::cout << "\nError durante la inferencia: " << exception.what() << std::endl;
  }
}

/**
 * @brief Ejecuta un análisis de rendimiento exhaustivo, probando diferentes configuraciones de variables de interés y condicionadas, mostrando estadísticas y permitiendo exportar los resultados.
 */
void UserInterface::runPerformanceAnalysis() {
  if (!checkDistributionLoaded()) { return; }
  
  int number_variables = distribution_->getNumberVariables();
  
  int maximum_interest_variables = readInt("Máximo de variables de interés a probar", 1, number_variables - 1);
  int maximum_conditioned_variables = readInt("Máximo de variables condicionadas a probar", 0, number_variables - maximum_interest_variables - 1);
  int repetitions = readInt("Repeticiones por configuración", 1, 20);
  
  analyzer_->clear();
  analyzer_->runAnalysis(*distribution_, maximum_interest_variables, maximum_conditioned_variables, repetitions);
  
  std::cout << std::endl;
  analyzer_->displayStatistics();
  
  if (readConfirmation("\n¿Exportar datos a CSV?")) {
    std::string filename = readString("Nombre del archivo CSV de salida");
    analyzer_->exportToCSV(filename);
  }
  
  if (readConfirmation("\n¿Generar el informe completo?")) {
    std::string filename = readString("Nombre del archivo de reporte de salida");
    analyzer_->generateReport(filename);
  }
}

/**
 * @brief Muestra la distribución actual por pantalla, verificando primero que una distribución esté cargada.
 */
void UserInterface::displayCurrentDistribution() {
  if (!checkDistributionLoaded()) { return; }
  distribution_->display();
}

/**
 * @brief Exporta la distribución actual a un archivo CSV, verificando primero que una distribución esté cargada y manejando posibles errores durante la exportación.
 */
void UserInterface::exportDistribution() {
  if (!checkDistributionLoaded()) { return; }
  
  std::string filename = readString("Nombre del archivo de salida");
  
  try {
    distribution_->exportToCSV(filename);
  } catch (const std::exception& exception) {
    std::cout << "Error al exportar: " << exception.what() << std::endl;
  }
}

/**
 * @brief Muestra una sección de ayuda detallada, explicando los conceptos clave, el formato de los archivos CSV y el algoritmo utilizado para calcular las probabilidades condicionales.
 */
void UserInterface::displayHelp() const {
  std::cout << "Este programa calcula probabilidades condicionales en distribuciones binarias discretas usando marginalización." << std::endl;
  
  std::cout << "Conceptos Clave:\n";
  std::cout << "  • Variables: Indexadas de 1 a N (internamente de 0 a N-1)" << std::endl;
  std::cout << "  • Variables de Interés: Variables cuya distribución se desea" << std::endl;
  std::cout << "  • Variables Condicionadas: Variables con valores fijos" << std::endl;
  std::cout << "  • Variables Marginalizadas: Todas las demás variables (sumadas)" << std::endl;
  std::cout << std::endl;
  
  std::cout << "Formato CSV:" << std::endl;
  std::cout << "  binary_mask,probability" << std::endl;
  std::cout << "  Ejemplo para N=3:" << std::endl;
  std::cout << "    000,0.10" << std::endl;
  std::cout << "    001,0.15" << std::endl;
  std::cout << "    ..." << std::endl;
  std::cout << std::endl;
  
  std::cout << "Algoritmo:" << std::endl;
  std::cout << "  P(X_I | X_C = c) = Σ P(X_I, X_C = c, X_M) / P(X_C = c)" << std::endl;
  std::cout << "  donde X_M son las variables marginalizadas." << std::endl;
  std::cout << std::endl;
}

/**
 * @brief Lee un número entero del usuario, mostrando un mensaje de error si la entrada no es válida o está fuera de rango, y repitiendo hasta obtener una entrada correcta.
 * @param[in] prompt: El mensaje a mostrar al solicitar la entrada.
 * @param[in] min_value: El valor mínimo permitido (inclusive).
 * @param[in] max_value: El valor máximo permitido (inclusive).
 * @return El número entero ingresado por el usuario, garantizado para estar dentro del rango
 */
int UserInterface::readInt(const std::string& prompt, int min_value, int max_value) {
  int value;
  while (true) {
    std::cout << prompt << " [" << min_value << "-" << max_value << "]: ";
    std::cin >> value;
    
    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Entrada inválida. Por favor, ingrese un número." << std::endl;
    } else if (value < min_value || value > max_value) {
      std::cout << "Valor fuera de rango." << std::endl;
    } else {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return value;
    }
  }
}

/**
 * @brief Lee una cadena de texto del usuario, mostrando un mensaje de solicitud.
 * @param[in] prompt: El mensaje a mostrar al solicitar la entrada.
 * @return La cadena de texto ingresada por el usuario.
 */
std::string UserInterface::readString(const std::string& prompt) {
  std::string value;
  std::cout << prompt << ": ";
  std::getline(std::cin, value);
  return value;
}

/**
 * @brief Lee una confirmación del usuario, mostrando un mensaje de solicitud y validando la entrada.
 * @param[in] prompt: El mensaje a mostrar al solicitar la confirmación.
 * @return true si el usuario confirma (ingresa 's' o 'S'), false en caso contrario.
 */
bool UserInterface::readConfirmation(const std::string& prompt) {
  std::cout << prompt << " (s/n): ";
  std::string response;
  std::getline(std::cin, response);
  return !response.empty() && (response[0] == 's' || response[0] == 'S');
}

/**
 * @brief Verifica que una distribución esté cargada antes de realizar operaciones que dependan de ella, mostrando un mensaje de error si no es así.
 * @return true si una distribución está cargada, false en caso contrario.
 */
bool UserInterface::checkDistributionLoaded() const {
  if (!distribution_) {
    std::cout << "No hay distribución cargada. Por favor, cargue o genere una primero." << std::endl;
    return false;
  }
  return true;
}
