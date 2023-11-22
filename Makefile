# Compilers
CC=g++
OMPFLAG=-fopenmp

# Flags for optimization, debugging, and libs
OPTFLAGS=-O3
DEBUGFLAGS=-g -DDEBUG 
LIBS=-lm -lpthread -ldl

# Targets to build
OBJS=ising_model integration_omp

all: $(OBJS)

ising_model: ising_model.cpp
	$(CC) $(if $(DEBUG),$(DEBUGFLAGS),$(OPTFLAGS)) $(CFLAGS) $(OMPFLAG) $^ $(LIBS) -o $@

integration_omp: integration_omp.cpp
	$(CC) $(if $(DEBUG),$(DEBUGFLAGS),$(OPTFLAGS)) $(CFLAGS) $(OMPFLAG) $^ $(LIBS) -o $@

# Remove the target files
clean:
	rm -rf $(OBJS)

# Compile in debug mode
debug:
	make all DEBUG=1
