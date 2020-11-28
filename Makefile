
EXECUTABLE := paracube

CC_FILES   := main.cpp cube.cpp solver.cpp

###########################################################

ARCH=$(shell uname | sed -e 's/-.*//g')

OBJDIR=objs
CXX=g++ -m64
CXXFLAGS=-Wall -O3 -std=c++11 -g

OBJS=$(OBJDIR)/main.o  $(OBJDIR)/cube.o $(OBJDIR)/solver.o

.PHONY: dirs clean

all: $(EXECUTABLE)

default: $(EXECUTABLE)

dirs:
		mkdir -p $(OBJDIR)/

clean:
		rm -rf $(OBJDIR) *~ $(EXECUTABLE)

$(EXECUTABLE): dirs $(OBJS)
		$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(OBJDIR)/%.o: src/%.cpp
		$(CXX) $< $(CXXFLAGS) -c -o $@

