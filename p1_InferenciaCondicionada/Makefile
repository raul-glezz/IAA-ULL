CXX := g++
CXXFLAGS := -std=c++23
LDFLAGS := -lstdc++

SRCDIR := src
OBJDIR := obj

# Descubre todos los .cc recursivamente bajo src
SRC := $(shell find $(SRCDIR) -name '*.cc')
# Genera los objetos correspondientes bajo obj, reflejando la estructura de directorios
OBJ := $(patsubst $(SRCDIR)/%.cc, $(OBJDIR)/%.o, $(SRC))

BIN := p1_InferenciaCondicionada

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ)
	@echo "Enlazando $^ --> $@"
	@$(CXX) $^ -o $@ $(LDFLAGS)

# Regla genérica: compila cualquier src/%.cc a obj/%.o, creando subdirectorios si hace falta
$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	@echo "Compilando $< --> $@"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -I$(SRCDIR) -c $< -o $@

run: $(BIN)
	./$(BIN)

visualize:
	python3 -m venv .venv
	.venv/bin/pip install -r visualizer/requirements.txt
	.venv/bin/python3 visualizer/visualize_performance.py output/results.csv

clean:
	@echo "Limpiando..."
	@rm -rf $(OBJDIR) $(BIN) .venv