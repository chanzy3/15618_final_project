
EXECUTABLE := paracube

CC_FILES   := main.cpp cube.cpp solver.cpp

###########################################################

ARCH=$(shell uname | sed -e 's/-.*//g')

OBJDIR=objs
CXX=g++ -m64
CXXFLAGS=-Wall -O3 -fopenmp -std=c++0x -g

OBJS=$(OBJDIR)/main.o  $(OBJDIR)/cube.o $(OBJDIR)/solver.o \
        $(OBJDIR)/solver_bfs.o \
        $(OBJDIR)/solver_ida_base.o \
        $(OBJDIR)/solver_ida_rec_omp.o \
        $(OBJDIR)/solver_ida_rec_seq.o \
        $(OBJDIR)/solver_ida_iter_base.o \
        $(OBJDIR)/solver_ida_iter_omp.o \
        $(OBJDIR)/solver_ida_iter_omp_base.o \
        $(OBJDIR)/solver_ida_iter_omp_main_worker.o \
        $(OBJDIR)/solver_ida_iter_seq.o \
        $(OBJDIR)/util/NibbleArray.o \
        $(OBJDIR)/util/RubiksCubeException.o \
        $(OBJDIR)/database/PatternDatabase.o \
        $(OBJDIR)/database/Korf/CornerPatternDatabase.o \
        # $(OBJDIR)/database/Korf/EdgeG1PatternDatabase.o \
        $(OBJDIR)/database/Korf/EdgeG2PatternDatabase.o \
        $(OBJDIR)/database/Korf/EdgePatternDatabase.o \
        $(OBJDIR)/database/Korf/EdgePermutationPatternDatabase.o \
        $(OBJDIR)/database/Korf/KorfPatternDatabase.o \

.PHONY: dirs clean

all: $(EXECUTABLE)

default: $(EXECUTABLE)

dirs:
		mkdir -p $(OBJDIR)/
		mkdir -p $(OBJDIR)/util
		mkdir -p $(OBJDIR)/database
		mkdir -p $(OBJDIR)/database/Korf

clean:
		rm -rf $(OBJDIR) *~ $(EXECUTABLE)

$(EXECUTABLE): dirs $(OBJS)
		$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(OBJDIR)/%.o: src/%.cpp
		$(CXX) $< $(CXXFLAGS) -c -o $@

