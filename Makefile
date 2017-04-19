polygon:polygon.o rect.o
	g++ polygon.o rect.o -o polygon
polygon.o:polygon.cpp rect.h test.hpp
	g++ -c polygon.cpp -o polygon.o
rect.o:rect.cpp rect.h
	g++ -c rect.cpp -o rect.o
clean:
	rm -rf *.o polygon
