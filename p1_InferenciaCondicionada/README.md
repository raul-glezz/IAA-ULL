# Práctica 1: Probabilidad Condicionada

**Universidad de La Laguna**  
**Escuela Superior de Ingeniería y Tecnología**  
**Grado en Ingeniería Informática**  
**Inteligencia Artificial Avanzada**

## Autores
- Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
- Enrique Gómez Díaz (alu0101550329@ull.edu.es)

## Descripción

Este proyecto implementa un motor de inferencia para distribuciones de probabilidad conjuntas sobre variables binarias discretas. Permite calcular distribuciones condicionales utilizando máscaras binarias para especificar las variables de interés y las condiciones.

## Estructura del Proyecto

```
p1_InferenciaCondicionada/
├── src/
│   ├── main.cc                                    # Programa de demostración
│   ├── distribution/
│   │   ├── i_distribution.h                       # Interfaz base
│   │   └── binary_distribution/
│   │       ├── binary_distribution.h              # Distribución binaria
│   │       └── binary_distribution.cc
│   └── conditional_inference_engine/
│       ├── conditional_inference_engine.h         # Motor de inferencia
│       └── conditional_inference_engine.cc
├── Makefile                                       # Sistema de compilación
└── README.md                                      # Este archivo
```

## Compilación

### Requisitos
- Compilador C++ con soporte para C++17 o superior
- Make

### Comandos de compilación

```bash
# Compilar el proyecto
make

# Compilar y ejecutar
make run

# Limpiar archivos de compilación
make clean

# Compilar con información de depuración
make debug
```

## Uso del Programa

El programa de demostración (`main.cc`) incluye 5 ejemplos que muestran:

### Ejemplo 1: Construcción de distribución binaria
Demuestra cómo crear una distribución sobre 2 variables y asignar probabilidades manualmente.

### Ejemplo 2: Generación aleatoria
Muestra la generación de distribuciones aleatorias y sus propiedades.

### Ejemplo 3: Inferencia Condicional Simple
Calcula `P(X1 | X2=1)` a partir de una distribución conjunta `P(X1, X2)`.

### Ejemplo 4: Inferencia con Múltiples Variables
Demuestra cálculo de `P(X1, X2 | X3=1)` sobre 3 variables.

### Ejemplo 5: Carga/Guardado de Distribuciones
Exporta e importa distribuciones desde archivos CSV.

## API Principal

### BinaryDistribution

```cpp
// Constructor con número de variables
BinaryDistribution(int numVariables);

// Constructor desde archivo CSV
BinaryDistribution(const std::string& filename);

// Obtener/establecer probabilidades
double getProbability(uint64_t state) const;
void setProbability(uint64_t state, double probability);

// Normalización y validación
void normalize();
bool isValid() const;

// Generación aleatoria
void generateRandom();

// Visualización y exportación
void display() const;
void exportToCSV(const std::string& filename) const;
```

### ConditionalInferenceEngine

```cpp
// Constructor
ConditionalInferenceEngine(const BinaryDistribution& jointDist);

// Cálculo de probabilidad condicional
// maskC: máscara de variables condicionadas
// valC:  valores de variables condicionadas
// maskI: máscara de variables de interés
// Retorna: array con distribución condicional (debe liberarse con delete[])
double* prob_cond_bin(uint64_t maskC, uint64_t valC, uint64_t maskI);
```

## Ejemplo de Uso

```cpp
// Crear distribución conjunta P(X1, X2)
BinaryDistribution joint(2);
joint.setProbability(0b00, 0.2);  // P(X1=0, X2=0)
joint.setProbability(0b01, 0.3);  // P(X1=0, X2=1)
joint.setProbability(0b10, 0.1);  // P(X1=1, X2=0)
joint.setProbability(0b11, 0.4);  // P(X1=1, X2=1)

// Crear motor de inferencia
ConditionalInferenceEngine engine(joint);

// Calcular P(X1 | X2=1)
uint64_t maskC = 0b01;  // Variable X2
uint64_t valC  = 0b01;  // X2=1
uint64_t maskI = 0b10;  // Variable X1

double* result = engine.prob_cond_bin(maskC, valC, maskI);

std::cout << "P(X1=0 | X2=1) = " << result[0] << std::endl;
std::cout << "P(X1=1 | X2=1) = " << result[1] << std::endl;

delete[] result;  // Importante: liberar memoria
```

## Representación de Estados

Los estados se representan como enteros de 64 bits donde cada bit corresponde a una variable:
- Bit 0 (LSB): Variable X1
- Bit 1: Variable X2
- Bit 2: Variable X3
- ...

Ejemplo para N=3 variables:
- `0b000` = (X1=0, X2=0, X3=0)
- `0b101` = (X1=1, X2=0, X3=1)
- `0b111` = (X1=1, X2=1, X3=1)

## Formato CSV

Los archivos CSV tienen el formato:
```
00,0.2000000000
01,0.3000000000
10,0.1000000000
11,0.4000000000
```

Cada línea contiene:
- Configuración binaria (sin espacios)
- Coma
- Probabilidad (formato decimal)

## Ejecución de Ejemplo

```bash
$ make run

╔════════════════════════════════════════════════════════════════════════════╗
║          PRÁCTICA 1: PROBABILIDAD CONDICIONADA                             ║
║          Demostración del Motor de Inferencia                              ║
╚════════════════════════════════════════════════════════════════════════════╝

[... salida de los ejemplos ...]

Todos los ejemplos se ejecutaron correctamente ✓
```

## Notas Técnicas

- Límite máximo: 64 variables binarias (restricción del tipo `uint64_t`)
- Complejidad temporal del cálculo condicional: O(2^N) donde N es el número total de variables
- Tolerancia numérica para validación: ε = 10^-9
- Gestión de memoria: el método `prob_cond_bin()` devuelve un array dinámico que debe ser liberado por el llamador

## Licencia

Ver archivo [LICENSE](LICENSE)
