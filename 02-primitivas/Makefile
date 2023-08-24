CC = g++

GLLIBS = -lglut -lGLEW -lGL

all: triangle.cpp primitives.cpp rectangle.cpp rectangle2.cpp 
	$(CC) triangle.cpp -o triangle $(GLLIBS)
	$(CC) primitives.cpp ../lib/utils.cpp -o primitives $(GLLIBS)
	$(CC) mouseClick.cpp ../lib/utils.cpp -o mouseClick $(GLLIBS)
	$(CC) rectangle.cpp ../lib/utils.cpp -o rectangle $(GLLIBS)
	$(CC) rectangle2.cpp ../lib/utils.cpp -o rectangle2 $(GLLIBS)
	$(CC) testingUtils.cpp ../lib/utils.cpp -o testingUtils $(GLLIBS)

clean:
	rm -f triangle primitives rectangle rectangle2 mouseClick testingUtils
