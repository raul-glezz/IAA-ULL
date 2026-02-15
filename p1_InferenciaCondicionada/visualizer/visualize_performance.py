#!/usr/bin/env python3
# Universidad de La Laguna
# Escuela Superior de Ingeniería y Tecnología
# Grado en Ingeniería Informática
# Inteligencia Artificial Avanzada
# Practica 1: Probabilidad Condicionada

# @file   performance_analyzer.cc
# @author Raúl Gonzalez Acosta (alu0101543529@ull.edu.es)
# @author Enrique Gómez Díaz (alu0101550329@ull.edu.es)
# @date   05/02/2026
# @brief  Script para visualizar el análisis de rendimiento del sistema de inferencia condicional.

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import sys
from pathlib import Path

def plot_performance_analysis(csv_file):
  # Leer datos
  try:
    df = pd.read_csv(csv_file)
  except FileNotFoundError:
    print(f"Error: Archivo '{csv_file}' no encontrado.")
    print("Primero ejecuta el análisis de rendimiento desde el programa.")
    return

  # Configurar estilo
  sns.set_style("whitegrid")
  plt.rcParams['figure.figsize'] = (15, 10)

  # Crear figura con subplots
  fig, axes = plt.subplots(2, 2, figsize=(15, 12))
  fig.suptitle('Análisis de Rendimiento - Sistema de Inferencia Condicional',
                fontsize=16, fontweight='bold')

  # 1. Tiempo vs Número de Variables de Interés
  ax1 = axes[0, 0]
  grouped = df.groupby('VariablesInteres')['TiempoEjecucion(us)'].agg(['mean', 'std'])
  grouped['mean'].plot(kind='bar', ax=ax1, yerr=grouped['std'], capsize=5, color='steelblue')
  ax1.set_title('Tiempo de Ejecución vs Variables de Interés')
  ax1.set_xlabel('Número de Variables de Interés')
  ax1.set_ylabel('Tiempo (μs)')
  ax1.grid(True, alpha=0.3)

  # 2. Tiempo vs Número de Variables Condicionadas
  ax2 = axes[0, 1]
  grouped = df.groupby('VariablesCondicionadas')['TiempoEjecucion(us)'].agg(['mean', 'std'])
  grouped['mean'].plot(kind='bar', ax=ax2, yerr=grouped['std'], capsize=5, color='coral')
  ax2.set_title('Tiempo de Ejecución vs Variables Condicionadas')
  ax2.set_xlabel('Número de Variables Condicionadas')
  ax2.set_ylabel('Tiempo (μs)')
  ax2.grid(True, alpha=0.3)

  # 3. Heatmap: Tiempo por combinación
  ax3 = axes[1, 0]
  pivot = df.pivot_table(
    values='TiempoEjecucion(us)',
    index='VariablesInteres',
    columns='VariablesCondicionadas',
    aggfunc='mean'
  )
  sns.heatmap(pivot, annot=True, fmt='.1f', cmap='YlOrRd', ax=ax3, cbar_kws={'label': 'Tiempo (μs)'})
  ax3.set_title('Heatmap: Tiempo de Ejecución')
  ax3.set_xlabel('Variables Condicionadas')
  ax3.set_ylabel('Variables de Interés')

  # 4. Estados Evaluados vs Tiempo
  ax4 = axes[1, 1]
  ax4.scatter(df['EstadosEvaluados'], df['TiempoEjecucion(us)'],
              alpha=0.6, c=df['VariablesInteres'], cmap='viridis', s=100)
  ax4.set_title('Estados Evaluados vs Tiempo de Ejecución')
  ax4.set_xlabel('Estados Evaluados')
  ax4.set_ylabel('Tiempo (μs)')
  ax4.set_xscale('log')
  ax4.set_yscale('log')
  ax4.grid(True, alpha=0.3)
  cbar = plt.colorbar(ax4.collections[0], ax=ax4)
  cbar.set_label('Variables de Interés')

  plt.tight_layout()

  # Guardar figura
  output_file = csv_file.replace('.csv', '_plots.png')
  plt.savefig(output_file, dpi=300, bbox_inches='tight')
  print(f"✓ Gráficos guardados en: {output_file}")

  # Mostrar figura
  plt.show()

  # Imprimir estadísticas
  print("\n=== ESTADÍSTICAS DE RENDIMIENTO ===\n")
  print(f"Total de mediciones: {len(df)}")
  print(f"Tiempo medio: {df['TiempoEjecucion(us)'].mean():.2f} μs")
  print(f"Tiempo mínimo: {df['TiempoEjecucion(us)'].min():.2f} μs")
  print(f"Tiempo máximo: {df['TiempoEjecucion(us)'].max():.2f} μs")
  print(f"Desviación estándar: {df['TiempoEjecucion(us)'].std():.2f} μs")

  print("\n=== ANÁLISIS POR VARIABLES DE INTERÉS ===\n")
  interest_stats = df.groupby('VariablesInteres')['TiempoEjecucion(us)'].describe()
  print(interest_stats)

  print("\n=== ANÁLISIS POR VARIABLES CONDICIONADAS ===\n")
  cond_stats = df.groupby('VariablesCondicionadas')['TiempoEjecucion(us)'].describe()
  print(cond_stats)

def main():
  if len(sys.argv) < 2:
    print("Uso: python3 visualize_performance.py <archivo_csv>")
    print("\nEjemplo:")
    print("  python3 visualize_performance.py performance_results.csv")
    sys.exit(1)

  csv_file = sys.argv[1]

  if not Path(csv_file).exists():
    print(f"Error: El archivo '{csv_file}' no existe.\n")
    print("Para generar datos:")
    print("1. Ejecuta el programa principal")
    print("2. Selecciona opción 4 (Performance Analysis)")
    print("3. Exporta los resultados a CSV")
    sys.exit(1)

  # Generar gráficos
  plot_performance_analysis(csv_file)
    
if __name__ == "__main__":
  main()
