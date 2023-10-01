CC = g++

GLLIBS = -lglut -lGLEW -lGL

all: mainDCEL.cpp 
	$(CC) mainDCEL.cpp dcel.cpp ../lib/utils.cpp  -o mainDCEL $(GLLIBS)

clean:
	rm -f mainDCEL 
