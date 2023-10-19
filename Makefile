# Makefile - Solver
# Escola politécnica - PUCPR
# Gabriel Cezário (gabriel.cezario@pucpr.edu.br)
# Milena Silvério (milena.silverio@pucpr.edu.br)

#
# Parâmetros:
#

PROJECT = ./solver

SOURCES.cxx = \
	SolverMain.cxx \
	Helpers/Helper.cxx \
	Representation/LinearSystems/Restriction.cxx \
	Representation/LinearSystems/System.cxx \
	Representation/Values/Number.cxx \

BINDIR = ./bin

# Derived variables

PROGRAM = $(PROJECT)
SOURCES = $(SOURCES.cxx)
OBJECTS = $(SOURCES:%.cxx=%.o)

# C++ Aditional Compliler and Linker Flags

CPPFLAGS +=
LDFLAGS +=

# Rules for C++.

.SUFFIXES: .cxx .o
COMPILE.cxx = $(CXX) $(CPPFLAGS) $(CCFLAGS)
LINK.cxx = $(CXX) $(LDFLAGS)
.cxx.o:
	@echo -e "Compilando $(notdir $<)"
	@$(COMPILE.cxx) -c -o $(<:%.cxx=%.o) $<

# Standard targets.

.KEEP_STATE:

all: $(PROGRAM)
objects: $(SOURCES) $(OBJECTS)
sources: $(SOURCES)
teste:
	@echo -e $(SOURCES)
$(PROGRAM): $(SOURCES.cxx) $(OBJECTS)
	@echo -e "Linkando $(notdir $@)"
	@$(LINK.cxx) -o $@ $(OBJECTS)
clean:
	@echo -e "Limpando: $(notdir $(OBJECTS) $(PROGRAM))"
	@rm -f  $(OBJECTS) core $(PROGRAM)
cleanall:
	@echo -e "Limpando tudo : $(notdir $(GENERATED))"
	@rm -f core $(GENERATED)
