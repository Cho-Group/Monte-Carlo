lattice3d:goal.o lattice.o point.o main.o
	g++ -o lattice3d lattice.o point.o main.o goal.o neighbor.h
main.o:main.cpp lattice.h point.h
	g++ -c main.cpp
lattice.o:lattice.h lattice.cpp point.h
	g++ -c lattice.cpp
point.o:point.h point.cpp
	g++ -c point.cpp
goal.o: goal.h goal.cpp
	g++ -c goal.cpp
clean:
	rm *.o
