TARGET=flight
CC=g++
#OPT=-Wall -g
OPT=-w -g
ifeq ($(shell uname),Darwin)
	LIB=-framework OpenGL -framework GLUT -framework Foundation
else
	LIB=-lglut -lGLU -lGL
endif
INC=

-include makefile.opt
.SUFFIXES:.cpp .hpp .c .o .h
SRC=$(shell ls *.cpp)
HED=$(shell ls *.hpp)
OBJ=$(SRC:.cpp=.o)

all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) $(OPT) -o $(TARGET) $(OBJ) $(LIB)
.cpp.o:
	$(CC) $(OPT) -c $< $(INC)
dep:
	g++ -MM -MG $(SRC) >makefile.depend
clean:
	rm -f $(TARGET) $(TARGET).exe
	rm -f *.o *.obj
	rm -f *~ *.~*
-include makefile.depend
