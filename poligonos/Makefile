CC = g++

GLLIBS = -lglut -lGLEW -lGL

all: mainPoligonos.cpp 
	$(CC) mainPoligonos.cpp poligono.cpp ../lib/utils.cpp  -o mainPoligonos $(GLLIBS)

clean:
	rm -f mainPoligonos 
