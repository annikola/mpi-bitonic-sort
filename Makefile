# Compiler and Flags
CC = mpicc
CFLAGS = -O2 -fopenmp -I include
LDFLAGS = -fopenmp

# Binary: mpi_test
SRC_mpi_bitonic_sort = src/mpi_bitonic_sort_prephase.c src/bitonic_sequence_helpers.c src/mpi_bitonic_sort.c
OBJ_mpi_bitonic_sort = $(SRC_mpi_bitonic_sort:src/%.c=build/%.o)
BIN_mpi_bitonic_sort = build/mpi_bitonic_sort

# Default Target: Build all binaries
all: $(BIN_mpi_bitonic_sort)

# Rule to Compile Object Files
build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to Link Binary
$(BIN_mpi_bitonic_sort): $(OBJ_mpi_bitonic_sort)
	$(CC) $(OBJ_mpi_bitonic_sort) -o $@ $(LDFLAGS)
	rm -rf build/*.o

# Clean Target
clean:
	rm -rf build
