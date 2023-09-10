CC = g++

GLLIBS = -lglut -lGLEW -lGL

all: triangle.cpp primitives.cpp rectangle.cpp rectangle2.cpp 
	$(CC) testingUtils.cpp ../lib/utils.cpp -o testingUtils $(GLLIBS)
	$(CC) aula01.cpp ../lib/utils.cpp -o aula01 $(GLLIBS)
	$(CC) aula02.cpp ../lib/utils.cpp -o aula02 $(GLLIBS)

clean:
	rm -f testingUtils aula01 aula02
