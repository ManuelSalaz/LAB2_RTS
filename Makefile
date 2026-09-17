CXX ?= g++
CROSS_CXX ?= aarch64-linux-gnu-g++
CXXFLAGS = -std=c++11 -ggdb3 -O0 -Wall -Wextra -pedantic -Iinclude

SRC_DIR = src
BIN_DIR = bin

.PHONY: all host rpi rpi_corregido clean

all: host rpi rpi_corregido

# Compilacion nativa para host (x86_64)
host: $(SRC_DIR)/sensor_data_processing.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/sensor_host $<

# Compilacion cruzada para Raspberry Pi 4 (ARM64 - Version con instrumentacion)
rpi: $(SRC_DIR)/sensor_data_processing.cpp
	@mkdir -p $(BIN_DIR)
	$(CROSS_CXX) $(CXXFLAGS) -static -o $(BIN_DIR)/sensor_rpi $<

# Compilacion cruzada para Raspberry Pi 4 (ARM64 - Version corregida sin fugas)
rpi_corregido: $(SRC_DIR)/sensor_data_processing_corregido.cpp
	@mkdir -p $(BIN_DIR)
	$(CROSS_CXX) $(CXXFLAGS) -static -o $(BIN_DIR)/sensor_rpi_corregido $<

clean:
	rm -f $(BIN_DIR)/sensor_host $(BIN_DIR)/sensor_rpi $(BIN_DIR)/sensor_rpi_corregido
