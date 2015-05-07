CXX = clang++
CXXFLAGS = -g -std=c++14 -Wall -pedantic
LDFLAGS =

#SRC = $(wildcard *.cpp)
#OBJ = $(SRC: %.cpp = %.o)

BIN = lisp

all: object.o context.o parser.o evaluate.o lisp.o
	$(CXX) $(LDFLAGS) -o $(BIN) $^

#$(EXE): $(OBJ)
#	$(CXX) $(LDFLAGS) $(OBJ) -o $@

#all: $(OBJ)
#	$(CXX) $(CXXFLAGS) -o $(BIN) $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
clean:
	rm -v *.o $(BIN)